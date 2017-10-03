#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
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
		for(j=0; j<strlen(psep); j++)
		{
			if(pbuf[i] == psep[j])
				return i;
		}
	}
	return -1;
}

int CheckLoginPassword(char* lg,char* password)
{/*
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
	return -1;*/
		
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
	memcpy(pbuf, &tmp, sizeof(int));
	memcpy(&pbuf[sizeof(int)], &psize, sizeof(int));
	memcpy(&pbuf[2*sizeof(int)], pmsg, psize);
	int ret = sendSize(phandle, pbuf, taille);
	free(pbuf);
	return ret;
}

char *receiveMsgRequest(int handle, TypeRequete *pt, int *psize)
{
	char *pbuf;
	pbuf = receiveSize(handle, 2*sizeof(int));
	*pt = (TypeRequete) atoi(pbuf);
	*psize = atoi(&(pbuf[sizeof(int)]));
	free(pbuf);
	pbuf = receiveSize(handle, *psize);
	//pbuf[(*psize)-1]='\0';
	return pbuf;
}

int random(int max, int min)
{
	return (rand()%(max-min))+min;
}























