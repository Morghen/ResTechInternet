#ifndef __LIBUTILS_H__
#define __LIBUTILS_H__


typedef enum TypeRequete 
{
   Connect = 1,
	Deconnect,
	Ack,
	Nok,
	CheckTicket,
	CheckLuggage,
	PayementDone,
}TypeRequete;

int waitTime(int sec, long nsec);

int checkSep(char *pbuf, int psize, char *psep);

void Identify();

int CheckLoginPassword(char* login,char* password);

int sendMsgRequest(int phandle, TypeRequete pt, char *pmsg, int psize);

char *receiveMsgRequest(int handle, TypeRequete *pt, int *psize);

int random(int min, int max);














#endif
