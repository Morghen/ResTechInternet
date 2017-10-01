#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "SocketException.h"
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
		throw SocketException(SocketException::ERRORINIT);
	}
	
	//acquisition info ordi
	if((infosHost = gethostbyname(""))==0)
	{
		//throw
		throw SocketException(SocketException::ERRORINIT);
	}
	memcpy(&adresseIP, infosHost->h_addr, infosHost->h_length);
	
	//prepa struct sockaddr_in
	memset(&adresseSocket, 0, sizeof(struct sockaddr_in));
	adresseSocket.sin_family = AF_INET;
	adresseSocket.sin_port = htons(pport);
	memcpy(&adresseSocket.sin_addr, infosHost->h_addr, infosHost->h_length);
	
	//bind
	if(bind(handleSocket, (struct sockaddr *)&adresseSocket, sizeof(struct sockaddr_in))== -1)
	{
		//throw
		throw SocketException(SocketException::ERRORINIT);
	}
	return handleSocket;
}

int ServerListen(int phandle)
{
	//
	if(listen(phandle, MAX_CONNECTION) == -1)
	{
		//throw
		throw SocketException(SocketException::ERRORLISTEN);
	}
	return 1;
}

int ServerAccept(int phandle, struct sockaddr_in *paddrsock)
{
	//
	int handleService;
	unsigned int taille = sizeof(struct sockaddr_in);
	if((handleService = accept(phandle, (struct sockaddr *)paddrsock, &taille))==-1)
	{
		//throw
		throw SocketException(ERRORACCEPT);
	}
	return handleService;
}

int ClientConnect(int phandle, struct sockaddr_in *paddrsock)
{
	//
	unsigned int taille = sizeof(struct sockaddr_in);
	if(connect(phandle, (struct sockaddr *)paddrsock, taille)==-1)
	{
		//throw
		throw SocketException(ERRORCONNECT);
	}
	return 1;
}

int ClientInit(void)
{
	//
	return 1;
}

int SendMsg(void)
{
	//
	return 1;
}

int RcvMsg(void)
{
	//
	return 1;
}

int CloseSocket(int pi)
{
	//
	return close(pi);
}





























