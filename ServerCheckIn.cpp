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
#include "libUtils.h"
#include "socketLib.h"
#include "SocketException.h"



using namespace std;

#define	NB_THREADS	5
#define	PORT			(50000)

pthread_t ThreadId[NB_THREADS] = {0};


int hancleService[NB_THREADS] = {0};

void *ThClient(void *);

int main()
{
	int handleServer;
	int handleTmp;
	struct sockaddr_in *paddrsock=NULL;
	try
	{
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






























