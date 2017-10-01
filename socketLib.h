#ifndef __SOCKETLIB_H__
#define __SOCKETLIB_H__

#define	MAX_CONNECTION		(5)


int ServerInit(int pport);

int ClientInit(void);

int SendMsg(void);

int RcvMsg(void);

int CloseSocket(int pi);

int ServerListen(int phandle);

int ServerAccept(int phandle, struct sockaddr_in *paddrsock);

int ClientConnect(int phandle, struct sockaddr_in *paddrsock);
























#endif
