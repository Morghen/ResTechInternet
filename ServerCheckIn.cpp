#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include "libUtils.h"
#include "socketLib.h"
#include "SocketException.h"



using namespace std;

#define	NB_THREADS	5
#define	PORT			(50000)
#define MAXSTRING		(500)


pthread_t ThreadId[NB_THREADS] = {0};



int handleService[NB_THREADS] = {0};
int nbHandleWait = 0;
pthread_mutex_t mutexHandle;
pthread_cond_t condHandle;


void *ThClient(void *);

int main()
{
	int handleServer;
	int handleTmp;
	int i;
	struct sockaddr_in *paddrsock=NULL;
	try
	{
		pthread_mutex_init(&mutexHandle, NULL);
		pthread_cond_init(&condHandle, NULL);
		cout << "Demarrage serveur" << endl;
		cout << "Demarrage pool de threads" << endl;
		for(int j=0;j<NB_THREADS;j++)
			pthread_create(&ThreadId[j],NULL,ThClient,NULL);
		cout << "Fin creation threads" << endl;
		
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
	cout << "Thread : " << pthread_self() << " lance" << endl;
	int handleS;
	char *msgRecv;
	char bufMsgSend[MAXSTRING] = {0};
	pthread_mutex_lock(&mutexHandle);
	TypeRequete typeCli, typeSer;
	int sizeCli, sizeSer;
	
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
	cout << "recup handle"<<endl;
	while(1)
	{
		//
		
		msgRecv = receiveMsgRequest(handleS, &typeCli, &sizeCli);
		bufMsg[sizeCli-1]='\0';
		cout<< "Serv "<<pthread_self()<<" received : "<<bufMsg<<endl;
		
		sprintf(bufMsgSend, "ACK serv : %s", bufMsg);
		
		sendSize(handleS, bufMsgSend, MAXSTRING);
		cout<< "Serv "<<pthread_self()<<" sended : "<<bufMsgSend<<endl;
		free(bufMsg);
	}
	cout << pthread_self() << " je meurs" << endl;
	pthread_exit(0);
}






























