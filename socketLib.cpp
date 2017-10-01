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

#define TAILLELECSEP (10)

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
	if((infosHost = gethostbyname("localhost"))==0)
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
	
	cout << inet_ntoa(adresseIP)<<":"<<pport<<endl;
	
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
		throw SocketException(SocketException::ERRORACCEPT);
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
		throw SocketException(SocketException::ERRORCONNECT);
	}
	return 1;
}

int ClientInit(int pport, struct sockaddr_in *adresseSocket)
{
	//
	struct hostent *infosHost;
	struct in_addr adresseIP;
	//struct sockaddr_in adresseSocket;
	int handleSocket;
	
	//creation socket
	if((handleSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		// throw
		throw SocketException(SocketException::ERRORINIT);
	}
	
	//acquisition info ordi
	if((infosHost = gethostbyname("localhost"))==0)
	{
		//throw
		throw SocketException(SocketException::ERRORINIT);
	}
	memcpy(&adresseIP, infosHost->h_addr, infosHost->h_length);
	
	
	
	//prepa struct sockaddr_in
	memset(adresseSocket, 0, sizeof(struct sockaddr_in));
	adresseSocket->sin_family = AF_INET;
	adresseSocket->sin_port = htons(pport);
	memcpy(&(adresseSocket->sin_addr), infosHost->h_addr, infosHost->h_length);
	
	cout << "clien: "<<inet_ntoa(adresseIP)<<":"<<pport<<endl;
	
	return handleSocket;
}

int SendMsg(int phandle )
{
	//
	return 1;
}

char *receiveSize(int phandle, int psize)
{
	//
	int tmpLec=0;
	int i;
	char *buf = (char *)malloc(psize*sizeof(char));
	for(i=0; i<psize; i += tmpLec)
	{
		tmpLec = recv(phandle, &(buf[i]), psize-i, 0);
		if(tmpLec == -1)
			throw SocketException("Error receive size too short");
	}
	if(i != psize)
		throw SocketException("Error receive size too short");
	//memcpy(pbuf, buf, sizeof(psize));
	//free(buf);
	return buf;
}

char *receiveSep(int phandle, char *psep)
{
	//
	int tmpLec=0;
	int i, isep=0;

	char tmpBuf[5500] = {0};
	bool fini = false;
	int tailleS, tailleO;
	tailleO = sizeof(int);
	int tailleMsg=0;
	
	getsockopt(phandle, IPPROTO_TCP, TCP_MAXSEG, &tailleS, (socklen_t *)&tailleO);
	
	char *buf = (char *)malloc(tailleS*sizeof(char));
	for(i=0; fini == false; i++)
	{
		tmpLec = recv(phandle, &(tmpBuf[0]), tailleS, 0);
		if(tmpLec == -1)
			throw SocketException("Erreur receive sep");
		if((isep = checkSep(&(tmpBuf[0]), tmpLec, psep)) >= 0)
		{
			fini = true;
		}
		else
		{
			buf = (char *) realloc(buf, tailleS * (i+2) * sizeof(char));
		}
		
		memcpy(&(buf[tailleMsg]), &tmpBuf, tmpLec);
		tailleMsg += tmpLec;
	}
	
	return buf;
}

int CloseSocket(int pi)
{
	//
	return close(pi);
}





























