#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <iostream>

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


































