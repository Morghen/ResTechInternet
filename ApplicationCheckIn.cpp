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
#include "Billets.h"

#define MAXSTRING		(500)

using namespace std;

void BilletsManager();
void AddBillet();


#define	PORT	50000

int main()
{
	// Variables
	char choix;
	struct sockaddr_in *adresseSocket = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
	int handleSocket;
	// Init et ouverture des sockets
	char *bufMsgReceive;
	char bufMsgSend[MAXSTRING] = {0};
	
	try
	{

		cout << "client socket init"<<endl;
		handleSocket = ClientInit(PORT, adresseSocket);
		
		cout << "client connect"<<endl;
		ClientConnect(handleSocket, adresseSocket);
		
		cout <<"client connecter !!!!!!"<<endl;	
		
		for(int i=0; i< 5; i++)
		{
			sprintf(bufMsgSend, "message n°%d sur 5",i);
			sendSize(handleSocket, bufMsgSend, MAXSTRING);
			cout << "client sended: "<<bufMsgSend<<endl;
			
			bufMsgReceive = receiveSize(handleSocket, MAXSTRING);
			cout << "client receive: "<<bufMsgReceive<<endl;
			free(bufMsgReceive);
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

void BilletsManager()
{
	char choix;
	cout << "*** Gestions des billets ***" << endl;
	cout << "1. Ajouter un billet" << endl;
	cout << "2. Changer de compte" << endl;
	cout << "3. Quitter" << endl;
	cout << "Votre choix : ";
	cin >> choix;
	switch(choix)
	{
		case '1':
			AddBillet(); // Ajout d'un billets
			break;
		case '2':
			return;
		default:
			exit(0);			
	}
	
}

void AddBillet()
{
	Billet newBillet();
	cout << "*** VOL 757 POWDER-AIRLINES - Kaboul 14h30 ***" << endl;
	cout << "Numéro de billet ?";
	
}





