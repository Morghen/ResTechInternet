#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <iostream>
#include <string.h>

using namespace std;



int waitTime(int sec, long nsec)
{
	if(sec < 0 || nsec < 0 || nsec > 999999999)
	{
		cout << "Erreur nanosleep" << endl;
		return -1; 
	}
	struct timespec time;
	time.tv_sec = sec;
	time.tv_nsec = nsec;
	return nanosleep(&time, NULL);
}

int checkSep(char *pbuf, int psize, char *psep)
{
	int i,j;
	for(i=0; i <psize; i++)
	{
		for(j=0; j<strlen(psep); j++)
		{
			if(pbuf[i] == psep[j])
				return i;
		}
	}
	return -1;
}































