#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include "SocketException.h"
#include "socketLib.h"
#include "libUtils.h"

using namespace std;



int waitTime(int sec, long nsec)
{
	if(sec < 0 || nsec < 0 || nsec > 999999999)
	{
		cout << "Erreur nanosleep" << endl;
		return -1; 
	}
	struct timespec time;
	time.tv_sec = sec;
	time.tv_nsec = nsec;
	return nanosleep(&time, NULL);
}


int checkSep(char *pbuf, int psize, char *psep)
{
	int i,j;
	for(i=0; i <psize; i++)
	{
		for(j=0; j<(int)strlen(psep); j++)
		{
			if(pbuf[i] == psep[j])
				return i;
		}
	}
	return -1;
}

int CheckLoginPassword(char* lg,char* password)
{
	// Variables
	fstream fs;
	char tmp[100]={0};
	
	// Ouverture du fichier csv
	try
	{
		fs.open("officer_list.csv",fstream::in);
	}
	catch(exception &e)
	{
		cout << "Erreur open fichier:" << e.what() << endl;
	}
	
	// Recherche du couple login-password	
	while(fs.peek() != EOF) // Recherche tant que EOF n'est pas atteint
	{
		fs.getline(tmp,100,';');
		cout << "Comparaison lg & tmp :" << lg << " & " << tmp << endl;
		if(strcmp(lg,tmp) == 0)
		{
			cout << "Login trouve,recherche mdp" << endl;
			fs.getline(tmp,100);
			cout << "Comparaison password & tmp :" << password << " & " << tmp << endl;
			if(strcmp(password,tmp) == 0)
			{
				cout << "Login & password correct" << endl;
				fs.close();
				return 0;
			}
			else
			{
				cout << "Password incorrect" << endl;
				fs.close();
				return -1;
			}
		}
		else
		{
			cout << "Login incorrect" << endl;
			fs.getline(tmp,100); // On passe le password
		}
	}
	cout << "Fin fichier atteint, aucune correspondance" << endl;
	fs.close();
	return -1;
}


void Identify()
{
	// Variables
	int loopAgain = 1;
	char login[50]={0},password[50]={0};
	// Recherche
	while(loopAgain == 1)
	{
		cout << "*** IDENTIFICATION ***" << endl;
		cout << "Login : ";
		cin >> login;
		cout << "Mot de passe : ";
		cin >> password;	
		if(CheckLoginPassword(login,password) == -1)	//Fct qui check le login-mdp (renvoie 0 si OK, renvoie -1 si erreur)
			cout << "Compte inconnu, verifiez votre login/mot de passe" << endl;
		else
		{
			cout << "Compte valide, bienvenue " << login << endl;
			loopAgain=0;
		}
	}
}

int sendMsgRequest(int phandle, TypeRequete pt, char *pmsg, int psize)
{
	int taille = psize+sizeof(int)+sizeof(char);
	char *pbuf = (char*)malloc(taille);
	int tmp = pt;
	//cout << "send : "<<pmsg<<endl;
	
	
	memcpy(pbuf, &tmp, sizeof(int));
	memcpy((pbuf+sizeof(int)), pmsg, psize);
	//cout << "send : "<< pbuf<<endl;
	
	
	
	pbuf[taille-1]='$';
	//cout << "send : "<< pbuf<<endl;
	
	int ret = sendSize(phandle, pbuf, taille);
	
	free(pbuf);
	return ret;
}

char *receiveMsgRequest(int handle, TypeRequete *pt, int *psize)
{
	char *pbuf;
	int size;
	char sep[2] = "$";
	pbuf = receiveSep(handle, sep, &size);
	//cout << "recv : "<<pbuf<<endl;
	*psize = size - sizeof(int);
	memcpy(pt,pbuf, sizeof(int));
	
	char *tbuf = (char*)malloc(*psize);
	memcpy(tbuf, pbuf+sizeof(int), *psize);
	tbuf[(*psize)-1]='\0';
	return tbuf;
}

int random(int min, int max)
{
	return (rand()%(max-min))+min;
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
			AddBillet(); // Ajout d'un billet
			break;
		case '2':
			return;
		default:
			exit(0);			
	}
	
}

void AddBillet()
{
	// Variables
	fstream fichiercsv;
	char numBillet[100];
	char nomFichier[100];
	int nbVoyageurs=0;
	float poidsBagages[20]={0};
	char typeBagage[20]={'X'};
	char paiementOK;
	// Encodage des données
	cout << "*** VOL 757 POWDER-AIRLINES - Kaboul 14h30 ***" << endl;
	cout << "Numéro de billet ? ";
	cin >> numBillet;
	strcpy(nomFichier,numBillet);
	// Creation et ouverture du fichiercsv
	try
	{
		fichiercsv.open(strcat(nomFichier,"_lug.csv"),fstream::out);
	}
	catch(exception &e)
	{
		cout << "Erreur creation du fichier lug.csv" << endl;
	}
	cout << "Nombre d'accompagnants ? ";
	cin >> nbVoyageurs;
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
		if(typeBagage[i] == 'O' || typeBagage[i] == 'o')
			fichiercsv << numBillet << "-1430-00" << i << ";" << "VALISE\n";
		else
			fichiercsv << numBillet << "-1430-00" << i << ";" << "PASVALISE\n";
	}
	//Fin encodage données & fermeture fichier
	fichiercsv.close();
	// Envoie requete CHECK_LUGGAGE et recupere tous les calculs
	cout << "*** Résumé du billet ***" << endl;
	cout << "Numéro du billet : " << numBillet << endl;
	cout << "Nombre d'accompagnants : " << nbVoyageurs << endl;
	cout << "Poids total bagages : " << getTotalWeight(poidsBagages) << " kg" << endl;
	cout << "Excédent poids : " << getExcessWeight(poidsBagages) << " kg" << endl;
	cout << "Supplément à payer : " << getAddedTaxes(getExcessWeight(poidsBagages)) << " EUR" << endl;
	cout << "Paiement effectué ? ";
	cin.ignore();
	cin >> paiementOK; // Envoie requete PAYMENT_DONE
}


float getTotalWeight(float *poidsBagages)
{
	float sum=0;
	int i=0;
	while(poidsBagages[i] != 0)
	{
		sum = sum + poidsBagages[i];
		i++;
	}
	return sum;
}

float getExcessWeight(float* poidsBagages)
{
	float excess=0;
	int i = 0;
	while(poidsBagages[i] != 0)
	{
		if(poidsBagages[i] > 20)
		{
			excess = excess + (poidsBagages[i] - 20);
		}
		i++;
	}
	return excess;
}

float getAddedTaxes(float excessWeight)
{
	return excessWeight*2.95;
}
























