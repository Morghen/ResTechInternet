#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "socketLib.h"


int ServerInit(int pport)
{
	//
	struct hostent *infosHost;
	struct in_addr adresseIP;
	struct sockaddr_in adresseSocket;
	int handleSocket;
	
	//creation socket
	if((handleSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		// throw
	}
	
	//acquisition info ordi
	if((infosHost = gethostbyname(""))==0)
	{
		//throw
	}
	memcpy(&adresseIP, infosHost->h_addr, infosHost->h_length);
	
	//prepa struct sockaddr_in
	memset(&adreseSocket, 0, sizeof(struct sockaddr_in));
	adresseSocket.sin_family = AF_INET;
	adresseSocket.sin_port = htons(pport);
	memcpy(&adresseSocket.sin_addr, infosHost->h_addr, infosHost->h_length);
	
	//bind
	if(bind(handleSocket, (struct cockaddr *)&adresseSocket, sizeof(struct sockaddr_in))== -1)
	{
		//throw
	}
	return handleSocket;
}

int ServerListen(int phandle)
{
	//
	if(listen(phandle, MAX_CONNECTION) == -1)
	{
		//throw
	}
	return 1;
}

int ServerAccept(int phandle, struct sockaddr_in *paddrsock)
{
	//
	int handleService;
	int taille = sizeof(struct sockaddr_in);
	if((handleService = accept(phandle, (struct sockaddr *)paddrsock, taille))==-1)
	{
		//throw
	}
	return handleService;
}

int ClientConnect(int phandle, struct sockaddr_in *paddrsock)
{
	//
	int taille = sizeof(struct sockaddr_in);
	if(connect(phandle, (struct sockaddr *)paddrsock, taille)==-1)
	{
		//throw
	}
}

int ClientInit(void)
{
	//
}

int SendMsg(void)
{
	//
}

int RcvMsg(void)
{
	//
	
}

int CloseSocket(int pi)
{
	//
	close(pi);
}





























