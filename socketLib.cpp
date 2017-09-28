#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socketLib.h"


int ServerInit(void)
{
	int handleSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(handleSocket==-1)
	{
		return -1; // throw
	}
}
int ClientInit(void)
{}
int SendMsg(void)
{}
int RcvMsg(void)
{}
int CloseSocket(void)
{}
