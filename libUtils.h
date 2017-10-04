#ifndef __LIBUTILS_H__
#define __LIBUTILS_H__


typedef enum TypeRequete 
{
   Connect = 1,
	Deconnect,
	Ack,
	Nok,
	CheckTicket,
	CheckLuggage_1,
	CheckLuggage_2,
	PayementDone,
}TypeRequete;

int waitTime(int sec, long nsec);

int checkSep(char *pbuf, int psize, char *psep);

int CheckLoginPassword(char* login,char* password);

int sendMsgRequest(int phandle, TypeRequete pt, char *pmsg, int psize);

char *receiveMsgRequest(int handle, TypeRequete *pt, int *psize);

int sendMsgRequest(int phandle, TypeRequete pt, char *pmsg, int psize, char finTrame);

char *receiveMsgRequest(int handle, TypeRequete *pt, int *psize, char finTrame);

int random(int min, int max);

void AddBillet();

float getTotalWeight(float* poidsBagages);

float getExcessWeight(float* poidsBagages);

float getAddedTaxes(float excessWeight);

int Check_Ticket(char* numBillet,int nbVoyageurs);














#endif
