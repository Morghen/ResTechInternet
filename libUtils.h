#ifndef __LIBUTILS_H__
#define __LIBUTILS_H__


int waitTime(int sec, long nsec);

int checkSep(char *pbuf, int psize, char *psep);

void Identify();

int CheckLoginPassword(char* login,char* password);




















#endif
