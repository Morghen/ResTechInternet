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

void BilletsManager();
void AddBillet();
float getTotalWeight(float* poidsBagages);
float getExcessWeight(float* poidsBagages);
float getAddedTaxes(float* poidsBagages);


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
			//AddBillet(); // Ajout d'un billet
			break;
		case '2':
			return;
		default:
			exit(0);			
	}
	
}
/*
void AddBillet()
{
	// Variables
	fstream fs;
	char numBillet[100];
	int nbVoyageurs=0;
	float poidsBagages[20]={0};
	char typeBagage[20]={'X'};
	char paiementOK;
	// Encodage des données
	cout << "*** VOL 757 POWDER-AIRLINES - Kaboul 14h30 ***" << endl;
	cout << "Numéro de billet ?";
	cin >> numBillet;
	// Creation et ouverture du fichiercsv
	try
	{
		fichiercsv.open(strcat(numBillet,"_lug.csv"),fstream::out);
	}
	catch(exception &e)
	{
		cout << "Erreur creation du fichier lug.csv" << endl;
	}
	cout << "Nombre d'accompagnants ?";
	cin >> nbVoyageurs;
	for(int i=0;i<20;i++)
	{
		cout << "Poids du bagage n°" << i+1 << " <Enter si fini>: ";
		cin.ignore();
		if(cin.get() == '\n')
			break;
		cin >> poidsBagages[i];
		cout << "Valise ? ";
		cin.ignore();
		cin >> typeBagage[i];
		if(typeBagage[i] == 'O' || typeBagage[i] == 'o')
			fichiercsv << numBillet << "-1430-00" << i << ";" << "VALISE\n";
		else
			fichiercsv << numBillet << "-1430-00" << i << ";" << "PASVALISE\n";
	}
	fichiercsv.close();
	cout << "*** Résumé du billet ***" << endl;
	cout << "Numéro du billet : " << numBillet << endl;
	cout << "Nombre d'accompagnants : " << nbVoyageurs << endl;
	cout << "Poids total bagages : " << getTotalWeight(poidsBagages) << endl;
	cout << "Excédent poids : " << getExcessWeight(poidsBagages) << endl;
	cout << "Supplément à payer : " << getAddedTaxes(poidsBagages) << endl;
	cout << "Paiement effectué ? ";
	cin.ignore();
	cin >> paiementOK;
}

*/
float getTotalWeight(float *poidsBagages)
{
	int i=0;
	float sum=0;
	while(*(poidsBagages+i) != 0)
	{
		sum = sum + *(poidsBagages + i);
		i++;
	}
	return sum;
}

float getExcessWeight(float* poidsBagages)
{
	return 0;
}

float getAddedTaxes(float* poidsBagages)
{
	return 0;
}



