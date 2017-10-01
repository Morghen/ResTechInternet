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
	struct sockaddr_in *paddrsock=NULL;
	try
	{
		pthread_mutex_init(&mutexHandle, NULL);
		pthread_cond_init(&condHandle, NULL);
		cout << "Demarrage serveur" << endl;
		cout << "Demarrage pool de threads" << endl;
		for(int i=0;i<NB_THREADS;i++)
			pthread_create(&ThreadId[i],NULL,ThClient,NULL);
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
			for(int i=0; i < NB_THREADS; i++)
			{
				if(handleService[i]!= 0 )
				{
					handleService[i] = handleTmp;
				}
			}
			pthread_mutex_unlock(&mutexHandle);
			
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
	waitTime(5,0);
	cout << pthread_self() << " je meurs" << endl;
	pthread_exit(0);
}






























