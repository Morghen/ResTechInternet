#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>




void Identify();
int CheckLoginPassword(char* login,char* password);




int main()
{
	// Connexion au socket toussa toussa
	Identify();



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
	int fd = 0;
	
	// Ouverture du fichier csv et recherche de la chaine correspondante
	if((fd = open("officer_list.csv",O_RDONLY)) == -1)
	{
		perror("Erreur fichier :");
		exit(-1);
	}
	
	
	
	
}





















