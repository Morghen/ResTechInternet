#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "libUtils.h"
#include "socketLib.h"
#include "SocketException.h"

#define MAXSTRING		(500)

using namespace std;


#define	PORT	50000

int main()
{
	// Variables
	char choix;
	struct sockaddr_in *adresseSocket = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
	int handleSocket;
	// Init et ouverture des sockets
	char *msgSend;
	char *msgRecv;
	TypeRequete typeCli, typeSer;
	int sizeCli, sizeSer;
	srand(time(NULL));
	
	try
	{

		cout << "client socket init"<<endl;
		handleSocket = ClientInit(PORT, adresseSocket);
		
		cout << "client connect"<<endl;
		ClientConnect(handleSocket, adresseSocket);
		
		cout <<"client connecter !!!!!!"<<endl;	
		
		for(int i=0; i< 5; i++)
		{
			sizeCli = random(10, 30);
			msgSend = (char *)malloc(sizeCli);
			memset(msgSend, 'a'+random(0, 10), sizeCli-1);
			msgSend[sizeCli-1]='\0';
			int tmpReq = random(1,5);
			sendMsgRequest(handleSocket, (TypeRequete)tmpReq, msgSend, sizeCli);
			cout << "client sended: "<< tmpReq << " : "<<msgSend<<endl;
			
			msgRecv = receiveMsgRequest(handleSocket, &typeSer, &sizeSer);
			cout << "client receive: "<<msgRecv<<endl;
			free(msgSend);
			free(msgRecv);
		}
		
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
	
	// Menu principal
	while(1)
	{
		cout << "*** MENU PRINCIPAL ***" << endl;
		cout << "1. Connexion" << endl;
		cout << "2. Quitter" << endl;
		cout << "Votre choix : ";
		cin >> choix;
		switch(choix)
		{
			case '1':
				Identify();	// Identification
				BilletsManager();	// Gestion des billets
				break;
			default:
				exit(0);			
		}
	}
	
	// Fermeture socket
	close(handleSocket);
	free(adresseSocket);
}

