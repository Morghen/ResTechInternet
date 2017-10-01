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


#define	PORT	50000

void Identify();
int CheckLoginPassword(char* login,char* password);




int main()
{
	struct sockaddr_in *adresseSocket = (sockaddr_in *) malloc(sizeof(struct sockaddr_in));
	int handleSocket;
	try
	{
	
		// Connexion au socket toussa toussa
		
		//Identify();
		
		cout << "client socket init"<<endl;
		handleSocket = ClientInit(PORT, adresseSocket);
		
		cout << "client connect"<<endl;
		ClientConnect(handleSocket, adresseSocket);
		
		cout <<"client connecter !!!!!!"<<endl;
		
		
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
	close(handleSocket);
	free(adresseSocket);
}

/*void Identify()
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
		//Fct qui check le login-mdp (renvoie 0 si OK, renvoie -1 si erreur)
		if(CheckLoginPassword(login,password)==-1)
			cout << "Compte inconnu, verifiez votre login/mot de passe" << endl;
		else
		{
			cout << "Compte valide, bienvenue " << login << endl;
			loopAgain=0;
		}
	}
}

int CheckLoginPassword(char* lg,char* password)
{
	// Variables
	fstream fs;
	
	// Ouverture du fichier csv et recherche de la chaine correspondante
	try
	{
		fs.open("officer_list.csv",in);
	}
	catch(Exception e)
	{
		e.
	}
	
	
	
	
}
*/




















