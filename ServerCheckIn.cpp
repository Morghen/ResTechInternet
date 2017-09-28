#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "libUtils.h"
#include "socketLib.h"



using namespace std;

#define NB_THREADS 5

pthread_t ThreadId[NB_THREADS] = {0};

void *ThClient(void *);

int main()
{
	cout << "Demarrage serveur" << endl;
	cout << "Demarrage pool de threads" << endl;
	for(int i=0;i<NB_THREADS;i++)
		pthread_create(&ThreadId[i],NULL,ThClient,NULL);
	cout << "Fin creation threads" << endl;
	waitTime(20,0);
	cout << "Fin serveur" << endl;
}

void *ThClient(void *)
{
	cout << "Thread : " << pthread_self() << " lance" << endl;
	waitTime(5,0);
	cout << pthread_self() << " je meurs" << endl;
	pthread_exit(0);
}
