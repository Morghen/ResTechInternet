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

void Identify();
void BilletsManager();

char *msgSend;
char *msgRecv;
TypeRequete typeCli, typeSer;
int sizeCli, sizeSer;

	int handleSocket;

int main()
{
	// Variables
	char choix;
	struct sockaddr_in *adresseSocket = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));

	// Init et ouverture des sockets
	srand(time(NULL));
	
	try
	{

		cout << "client socket init"<<endl;
		handleSocket = ClientInit(PORT, adresseSocket);
		
		cout << "client connect"<<endl;
		ClientConnect(handleSocket, adresseSocket);
		
		cout <<"client connecter !!!!!!"<<endl;	
		
		/*for(int i=0; i< 5; i++)
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
		}*/
		
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
	
	
	// Fermeture socket
	close(handleSocket);
	free(adresseSocket);
}

void Identify()
{
	// Variables
	int loopAgain = 1;
	char login[50]={0},password[50]={0};
	char typerequete[20];
	// Recherche
	while(loopAgain == 1)
	{
		cout << "*** IDENTIFICATION ***" << endl;
		cout << "Login : ";
		cin >> login;
		cout << "Mot de passe : ";
		cin >> password;
	 	sprintf(typerequete,"%s,%s",login,password);
		sendMsgRequest(handleSocket,Connect,typerequete,strlen(typerequete));
		receiveMsgRequest(handleSocket,&typeSer,&sizeSer);
		if(typeSer == Nok)	//Fct qui check le login-mdp (renvoie 0 si OK, renvoie -1 si erreur)
			cout << "Compte inconnu, verifiez votre login/mot de passe" << endl;
		else
		{
			cout << "Compte valide, bienvenue " << login << endl;
			loopAgain=0;
		}
	}
}


void BilletsManager()
{
	char choix;
	cout << "*** Gestions des billets ***" << endl;
	cout << "1. Ajouter un billet" << endl;
	cout << "2. Changer de compte" << endl;
	cout << "6. Quitter" << endl;
	cout << "Votre choix : ";
	cin >> choix;
	switch(choix)
	{
		case '1':
			AddBillet(); // Ajout d'un billet
			break;
		case '2':
			//change de compte
			return;
		default:
			exit(0);			
	}
	
}















