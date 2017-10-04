#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
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
#define	PORT			(50000)
#define MAXSTRING		(500)


pthread_t ThreadId[NB_THREADS] = {0};

void HandlerSIGINT(int s);

int handleService[NB_THREADS] = {0};
int nbHandleWait = 0;
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
			
			fichierconf.open("server_checkin.conf",fstream::in);
			fichierconf.ignore(10000, '=');
		}
		catch(...)
		{
			//
			if(fichierconf.is_open() == false)
			{
				fichierconf.open("server_checkin.conf",fstream::out);
				fichierconf << "Port_Service=70000"<<endl<<"Port_Admin=70009"<<endl;
				fichierconf << "fin-trame=#"<<endl<<"sep-csv=;"<<endl;
				portServer = 70000;
				sepTrame = ';';
				finTrame = '#';
			}
		}
		
		
		
		//creation socket et handle avec bind
		
		handleServer = ServerInit(PORT);
		
		while(1)
		{
			cout <<"listen"<<endl;
			ServerListen(handleServer);
			
			cout << "accept"<<endl;
			handleTmp = ServerAccept(handleServer, paddrsock);
			cout << "client accepter"<<endl;
			
			//donner le handleTmp à un thread
			pthread_mutex_lock(&mutexHandle);
	
			for(i=0; i < NB_THREADS; i++)
			{
				if(handleService[i]== 0 )
				{
					handleService[i] = handleTmp;
					nbHandleWait++;
					i = NB_THREADS +1;
				}
			}
			pthread_mutex_unlock(&mutexHandle);
			pthread_cond_signal(&condHandle);
			
			if(i == NB_THREADS)
			{
				//répondre qu'on ne peut pas le prendre
				close(handleTmp);
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
		
		TypeRequete typeCli, typeSer;
		int sizeCli, sizeSer;
	
		pthread_mutex_lock(&mutexHandle);
		while(nbHandleWait == 0)
			pthread_cond_wait(&condHandle,&mutexHandle);
	
		for(int i=0; i<NB_THREADS; i++)
		{
			if(handleService[i] != 0)
			{
				handleS = handleService[i];
				handleService[i] = 0;
			}
		}
		pthread_mutex_unlock(&mutexHandle);
		//cout << "recup handle"<<endl;
		while(1)
		{
			//
			msgRecv = receiveMsgRequest(handleS, &typeCli, &sizeCli);
			//bufMsg[sizeCli-1]='\0';
			cout<< "Serv "<<pthread_self()<<" received : "<<msgRecv<<endl;
		
			switch(typeCli)
			{
				case Connect:
					//
					cout << "connect"<<endl;
					typeSer = Ack;
					tmpLogin = strtok(msgRecv, ",");
					tmpMdp = strtok(NULL,",");
					cout << tmpLogin << "  -- " <<tmpMdp<<endl;
					if(CheckLoginPassword(tmpLogin, tmpMdp) == -1)
						typeSer = Nok;
					break;
				case Deconnect:
					//
					cout << "deconnect"<<endl;
					typeSer = Ack;
					break;
				case CheckTicket:
					//
					cout << "check ticket"<<endl;
					typeSer = Ack;
					break;
				case CheckLuggage:
					//
					cout << "check luggage"<<endl;
					typeSer = Ack;
					break;
				case PayementDone:
					//
					cout << "payement"<<endl;
					typeSer = Ack;
					break;
				default:
					typeSer = Nok;
					cout << "default"<<endl;
					break;
			}
			sizeSer = strlen(msgRecv)+11;
			msgSend = (char*)malloc(sizeSer);
			sprintf(msgSend, "ACK serv : %s",msgRecv);
		
			sendMsgRequest(handleS, typeSer, msgSend, sizeSer);
			cout<< "Serv "<<pthread_self()<<" sended : "<<msgSend<<endl;
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
		exit(0);
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




























