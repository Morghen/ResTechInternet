#include <stdio.h>
#include <stdlib.h>
#include <iostream>



void Identify();
int CheckLoginPassword(char* login,char* password);




int main()
{
	int loopAgain = 1;
	while(loopAgain == 1)
	{
		cout << "*** IDENTIFICATION ***" << endl;
		cout << "Login : ";
		cin >> login;
		cout << "Mot de passe : ";
		cin >> password;
		//Fct qui check le login-mdp (renvoie 0 si OK, renvoie -1 si erreur)
		if(CheckLoginPassword==-1)
			cout << "Compte inconnu, verifier votre login/mot de passe" << endl;

	}



}


