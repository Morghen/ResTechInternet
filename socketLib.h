#ifndef __SOCKETLIB_H__
#define __SOCKETLIB_H__

#define	MAX_CONNECTION		(5)


int ServerInit(int pport);

int ClientInit(void);

int SendMsg(void);

int RcvMsg(void);

int CloseSocket(int pi);


























#endif
