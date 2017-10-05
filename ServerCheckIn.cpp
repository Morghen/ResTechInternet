#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <signal.h>
#include "libUtils.h"
#include "socketLib.h"
#include "SocketException.h"



using namespace std;

#define	NB_THREADS	5
#define MAXSTRING		(500)


pthread_t ThreadId[NB_THREADS] = {0};

void HandlerSIGINT(int s);

int handleService[NB_THREADS] = {0};
int nbHandleWait = -1;
pthread_mutex_t mutexHandle;
pthread_cond_t condHandle;
int handleServer;

char finTrame;
char sepTrame;
int portServer;

void *ThClient(void *);

int main()
{
	
	int handleTmp;
	int i;
	struct sockaddr_in *paddrsock=NULL;
	try
	{
		//sig int handler
		struct sigaction sigAct;

	  	sigAct.sa_handler = HandlerSIGINT;
	  	sigAct.sa_flags = 0;
	  	sigemptyset(&sigAct.sa_mask);
		sigaction(SIGINT,&sigAct,NULL); 
	
	
		pthread_mutex_init(&mutexHandle, NULL);
		pthread_cond_init(&condHandle, NULL);
		cout << "Demarrage serveur" << endl;
		cout << "Demarrage pool de threads" << endl;
		for(int j=0;j<NB_THREADS;j++)
			pthread_create(&ThreadId[j],NULL,ThClient,NULL);
		//cout << "Fin creation threads" << endl;
		
		//optention info
		fstream fichierconf;
		try
		{
			string strbuf;
			fichierconf.open("server_checkin.conf",fstream::in);
			fichierconf.ignore(1000, '=');
			fichierconf >> portServer;
			fichierconf.ignore(1000, '=');
			fichierconf.ignore(1000, '=');
			fichierconf >> sepTrame;
			fichierconf.ignore(1000, '=');
			fichierconf >> finTrame;
		}
		catch(...)
		{
			//
			
		}
		if(fichierconf.is_open() == false)
		{
			fichierconf.open("server_checkin.conf",fstream::out);
			fichierconf << "Port_Service=50000"<<endl<<"Port_Admin=50009"<<endl;
			fichierconf << "sep-trame=$"<<endl;
			fichierconf << "fin-trame=#"<<endl<<"sep-csv=;"<<endl;
			portServer = 50000;
			sepTrame = '$';
			finTrame = '#';
		}
		cout << "server config "<<endl;
		cout <<" port = "<<portServer<<endl;
		cout << "fin trame = "<<finTrame<<endl;
		cout << "sep trame = "<<sepTrame<<endl;
		
		
		//creation socket et handle avec bind
		
		handleServer = ServerInit(portServer);
		
		while(1)
		{
			cout <<"listen"<<endl;
			ServerListen(handleServer);
			
			cout << "accept"<<endl;
			handleTmp = ServerAccept(handleServer, paddrsock);
			
			
			//donner le handleTmp à un thread
			pthread_mutex_lock(&mutexHandle);
	
			for(i=0; i < NB_THREADS; i++)
			{
				cout << handleService[i] << " - ";
				if(handleService[i] == 0 )
				{
					handleService[i] = handleTmp;
					nbHandleWait = i;
					i = NB_THREADS ;
					cout << "client accepter"<<endl;
				}
			}
			cout <<endl;
			pthread_mutex_unlock(&mutexHandle);
			pthread_cond_signal(&condHandle);
			
			if(i == NB_THREADS)
			{
				//répondre qu'on ne peut pas le prendre
				shutdown(handleTmp, SHUT_RDWR);
				close(handleTmp);
				cout << "client refuser"<<endl;
			}
		}
		
		
		
		waitTime(20,0);
	
		cout << "Fin serveur" << endl;
	}
	catch(SocketException &e)
	{
		cout <<"Erreur socket : " << e.getMsg() << " n° : " << e.getNbrErr()<<endl;
	}
	catch(...)
	{
		cout << "Erreur inconnue "<< endl;
		perror(" t");
		exit(0);
	}
	close(handleServer);
}

void *ThClient(void *)
{
	try
	{
		cout << "Thread : " << pthread_self() << " lance" << endl;
		int handleS;
		char *msgRecv, *msgSend;
		char *tmpLogin, *tmpMdp;
		fstream fichiercsv;
		TypeRequete typeCli, typeSer;
		int sizeCli, sizeSer;
		int nbHandle;
	
		pthread_mutex_lock(&mutexHandle);
		while(nbHandleWait == -1)
			pthread_cond_wait(&condHandle,&mutexHandle);
		
		handleS = handleService[nbHandleWait];
		nbHandle = nbHandleWait;
		nbHandleWait=-1;
		
	
		pthread_mutex_unlock(&mutexHandle);
		//cout << "recup handle"<<endl;
		float *pdsBaggage=NULL;
		char *valise=NULL;
		char *numBillet;
		int login=0;
		int nbrBaggage=0;
		
		while(1)
		{
			//
			msgRecv = receiveMsgRequest(handleS, &typeCli, &sizeCli, finTrame);
			//bufMsg[sizeCli-1]='\0';
			cout<< "Serv "<<pthread_self()<<" received : "<<msgRecv<<endl;
			char sepTmp[2];
			sprintf(sepTmp, "%c", sepTrame);
			if(pdsBaggage == NULL)
			{
				pdsBaggage = (float*)malloc(sizeof(float));
			}

			float pdsEnTrop=0;
			float resteAPayer=0;
			float pdsTot=0;

			switch(typeCli)
			{
				case Connect:
					//
					cout <<"Serv "<<pthread_self()<< "connect"<<endl;
					typeSer = Ack;
					
					tmpLogin = strtok(msgRecv, sepTmp);
					tmpMdp = strtok(NULL,sepTmp);
					cout << tmpLogin << "  -- " <<tmpMdp<<endl;
					if(CheckLoginPassword(tmpLogin, tmpMdp) == -1)
						typeSer = Nok;
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "ACK serv");
					login = 1;
					break;
				case Deconnect:
					//
					cout <<"Serv "<<pthread_self()<< "deconnect"<<endl;
					typeSer = Ack;
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "ACK serv");
					login =0;
					break;
				case CheckTicket:
					//
					cout <<"Serv "<<pthread_self()<< "check ticket"<<endl;
					if(login == 1)
					{
						char *tmpStr = strtok(msgRecv, sepTmp);
						numBillet = (char*) malloc(strlen(tmpStr)+1);
						strcpy(numBillet, tmpStr);
						int nbPassager;
						nbPassager = atoi(strtok(NULL,sepTmp));
						if(Check_Ticket(numBillet, nbPassager) == 0)
						{
							//valide
							typeSer = Ack;
						}
						else
						{
							//FAUX
							typeSer = Nok;
						}
					}
					else
					{
						typeSer = Nok;
					}
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "ACK serv");
					break;
				case CheckLuggage_1:
					//
					cout <<"Serv "<<pthread_self()<< "check luggage"<<endl;
					if(login == 1)
					{
						char *tmpStr ;
						tmpStr= strtok(msgRecv, sepTmp);
						int i;
						for(i=0; tmpStr != NULL;i++)
						{
							pdsBaggage = (float*) realloc(pdsBaggage, sizeof(float)*(i+1));
							pdsBaggage[i] = atof(tmpStr);
							tmpStr = strtok(NULL, sepTmp);
							pdsTot += pdsBaggage[i];
							if(pdsBaggage[i] > 20)
								pdsEnTrop += pdsBaggage[i] - 20.0;
						}
						resteAPayer = pdsEnTrop*2.95;
						nbrBaggage = i;
						cout << "nbreBaggage : " << nbrBaggage << endl;
						
						typeSer = Ack;
						sizeSer = 3*sizeof(float)+2*sizeof(char);
						msgSend = (char*)malloc(sizeSer);
					
						memcpy(msgSend, &pdsTot, sizeof(float));
						msgSend[sizeof(float)]=sepTrame;
						memcpy(&(msgSend[sizeof(float)+sizeof(char)]), &pdsEnTrop, sizeof(float));
						msgSend[sizeof(float)*2+sizeof(char)]=sepTrame;
						memcpy(&(msgSend[2*sizeof(float)+2*sizeof(char)]), &resteAPayer, sizeof(float));
						cout << "valeur server"<<endl;
						cout << pdsTot<<endl;
						cout << pdsEnTrop<<endl;
						cout <<resteAPayer<<endl;
					}
					else
					{
						typeSer = Nok;
					}
					break;
				case CheckLuggage_2:
					//
					cout <<"Serv "<<pthread_self()<< "check type bagages"<<endl;
					if((valise == NULL) && (nbrBaggage > 0) && (login == 1))
					{
						if(valise != NULL)
							free(valise);
						valise = (char*)malloc(sizeof(char)*(nbrBaggage));
						typeSer = Ack;
						//
						int i;
						char *tmpStr;
						tmpStr = strtok(msgRecv, sepTmp);
						for(i=0; i< nbrBaggage  && tmpStr != NULL; i++)
						{
							
							
							valise[i] = tmpStr[0];
							tmpStr = strtok(NULL, sepTmp);
						}
					}
					else
					{
						cout << "passe par Nok dans CheckLuggage_2" << endl;
						typeSer = Nok;
					}
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "ACK serv");
					break;
				case PayementDone:
					//
					cout <<"Serv "<<pthread_self()<< "payement"<<endl;
					
					if(nbrBaggage > 0 && numBillet != NULL && login == 1)
					{
						cout << "msgRecv[0] "<<msgRecv[0]<<endl;
						if(msgRecv[0] == 'y' || msgRecv[0]=='Y')
						{
							typeSer = Ack;
							try
							{
								fichiercsv.open(strcat(numBillet,"_lug.csv"),fstream::out);
							}
							catch(exception &e)
							{
								cout << "Erreur creation du fichier lug.csv" << endl;
							}
							for(int i=0;i<nbrBaggage;i++)
							{
								if(valise[i] == 'O' || valise[i] == 'o')
									fichiercsv << numBillet << "-1430-00" << i << ";" << "VALISE\n";
								else
									fichiercsv << numBillet << "-1430-00" << i << ";" << "PASVALISE\n";
							}
							if(fichiercsv.is_open() == true)
								fichiercsv.close();
						}
						else
						{
							typeSer = Nok;
						}
					}
					else
					{
						typeSer = Nok;
					}
					/*if(pdsBaggage != NULL)
						free(pdsBaggage);
					if(valise != NULL)
						free(valise);*/
					pdsBaggage = NULL;
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "ACK serv");
					break;
				default:
					typeSer = Nok;
					sizeSer = 8*sizeof(char);
					msgSend = (char*)malloc(sizeSer);
					sprintf(msgSend, "Nok serv");
					cout << "default"<<endl;
					break;
			}
			
			//sprintf(msgSend, "ACK serv : %s",msgRecv);
		
			sendMsgRequest(handleS, typeSer, msgSend, sizeSer, finTrame);
			//cout<< "Serv "<<pthread_self()<<" sended : "<<msgSend<<endl;
			free(msgRecv);
			free(msgSend);
			
		
		}
		cout << pthread_self() << " je meurs" << endl;
		pthread_exit(0);
	}
	catch(SocketException &e)
	{
		cout <<"Erreur socket : " << e.getMsg() << " n° : " << e.getNbrErr()<<endl;
		perror(" ");
	}
	catch(...)
	{
		cout << "Erreur inconnue "<< endl;
		perror(" t");
		pthread_exit(0);
	}
	return NULL;
}

void HandlerSIGINT(int s)
{
	pthread_mutex_lock(&mutexHandle);
	for(int i =0; i<NB_THREADS; i++)
	{
		if(handleService[i] != 0)
			close(handleService[i]);
	}
	close(handleServer);
	pthread_mutex_unlock(&mutexHandle);
}




























