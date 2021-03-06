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
#include <signal.h>
#include "libUtils.h"
#include "socketLib.h"
#include "SocketException.h"

#define MAXSTRING		(500)

using namespace std;

void Identify();
void BilletsManager();
void AddBillet();

void HandlerSIGINT(int s);


char *msgSend;
char *msgRecv;
TypeRequete typeCli, typeSer;
int sizeCli, sizeSer;

	int handleSocket;
	
	
int portServer;
char sepTrame;
char finTrame;

int main()
{
	// Variables
	char choix;
	struct sockaddr_in *adresseSocket = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));

	// Init et ouverture des sockets
	srand(time(NULL));
	
	//sig int handler
		struct sigaction sigAct;

	  	sigAct.sa_handler = HandlerSIGINT;
	  	sigAct.sa_flags = 0;
	  	sigemptyset(&sigAct.sa_mask);
		sigaction(SIGINT,&sigAct,NULL); 
	
	try
	{
	
		//fichier config
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
		
		cout << "client socket init"<<endl;
		handleSocket = ClientInit(portServer, adresseSocket);
		
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
					close(handleSocket);
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
	 	sprintf(typerequete,"%s%c%s",login,sepTrame,password);
		sendMsgRequest(handleSocket,Connect,typerequete,strlen(typerequete),finTrame);
		receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
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
	while(1)
	{
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
				char typerequete[20];
				sprintf(typerequete,"Deconnect");
				sendMsgRequest(handleSocket,Deconnect,typerequete,strlen(typerequete),finTrame);	//change de compte
				receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
				return;
			default:
				close(handleSocket);
				exit(0);			
		}
	}
}

void AddBillet()
{
	// Variables
	fstream fichiercsv;
	char numBillet[100];
	char nomFichier[100];
	int nbVoyageurs=0;
	char typerequete[20];
	float poidsBagages[20]={0};
	char typeBagage[20]={'X'};
	char paiementOK;
	int nbrBaggage =0;
	// Encodage des données
	cout << "*** VOL 757 POWDER-AIRLINES - Kaboul 14h30 ***" << endl;
	cout << "Numéro de billet ? ";
	cin >> numBillet;
	strcpy(nomFichier,numBillet);
	cout << "Nombre d'accompagnants ? ";
	cin >> nbVoyageurs;
	sprintf(typerequete,"%s%c%d",numBillet,sepTrame,nbVoyageurs);
	sendMsgRequest(handleSocket,CheckTicket,typerequete,strlen(typerequete),finTrame);
	receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
	if(typeSer == Nok)
	{
		cout << "Billet introuvable ou nbVoyageurs incorrect" << endl;
		return;
	}	
	else
		cout << "Billet " << numBillet << " valide" << endl;
	// Encodage info
	for(int i=0;i<20;i++)
	{
		cout << "Poids du bagage n°" << i+1 << " <Enter si fini>: ";
		cin.ignore();
		if(cin.peek() == '\n')
			break;
		cin >> poidsBagages[i];
		cout << "Valise ? ";
		cin.ignore();
		cin >> typeBagage[i];
	}
	// Envoie requete CHECK_LUGGAGE et recupere tous les calculs
	int i=0, j=0;
	char typerequete2[1000]={'\0'};
	while(poidsBagages[i] != 0)
	{
		sprintf(typerequete2,"%s%f%c",typerequete2,poidsBagages[i],sepTrame);
		i++;
	}
	nbrBaggage = i;
	poidsBagages[i-1]='\0';
	sendMsgRequest(handleSocket,CheckLuggage_1,typerequete2,strlen(typerequete2),finTrame);
	//receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
	// Recupération info bagages
	char* infoBaggages;
	infoBaggages = receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
	cout << "msg recu serveur "<<infoBaggages<<endl;
	float pdsTot,pdsExces,pdsTaxes;
	memcpy(&pdsTot,infoBaggages,sizeof(float));
	memcpy(&pdsExces,&(infoBaggages[sizeof(float)+sizeof(char)]),sizeof(float));
	memcpy(&pdsTaxes,&(infoBaggages[2*sizeof(float)+2*sizeof(char)]),sizeof(float));
	memset(typerequete2, '\0', strlen(typerequete2));
	while(typeBagage[j] != 'X' && j < nbrBaggage)
	{
		sprintf(typerequete2,"%s%c%c",typerequete2,typeBagage[j],sepTrame);
		j++;
	}
	typeBagage[j-1]='\0';
	sendMsgRequest(handleSocket,CheckLuggage_2,typerequete2,strlen(typerequete2),finTrame);
	receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
	// Affichage du résumé
	cout << "*** Résumé du billet ***" << endl;
	cout << "Numéro du billet : " << numBillet << endl;
	cout << "Nombre d'accompagnants : " << nbVoyageurs << endl;
	cout << "Poids total bagages : " << pdsTot << " kg" << endl;
	cout << "Excédent poids : " << pdsExces << " kg" << endl;
	cout << "Supplément à payer : " << pdsTaxes << " EUR" << endl;
	cout << "Paiement effectué ? ";
	cin.ignore();
	cin >> paiementOK; // Envoie requete PAYMENT_DONE
	if(paiementOK == 'y' || paiementOK == 'Y')
		typerequete[0] = 'Y';
	else
		typerequete[0] = 'N';
	sendMsgRequest(handleSocket,PayementDone,typerequete,sizeof(char),finTrame);
	receiveMsgRequest(handleSocket,&typeSer,&sizeSer,finTrame);
}



void HandlerSIGINT(int s)
{
	
	close(handleSocket);
	exit(0);
	
}












