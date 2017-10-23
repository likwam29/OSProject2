#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//counters
int elfCounter = 0;
int deerCounter = 0;

//locks
pthread_mutex_t counterLock; // Locks both the elf and deer counter
pthread_mutex_t elfHelpLock; // keeps other elfs out while 3 are being helped by santa

//Condition Variables
pthread_cond_t santaCond; //Santa waits here
pthread_cond_t deerCond;  //deer wait here
pthread_cond_t elfCond;   //elfs wait here


int main(){
	
	pthread_t santa, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, 
				r1, r2, r3, r4, r5, r6, r7, r8, r9;	
	
	//init lock and CVs
	pthread_mutex_init(&counterLock, NULL);
	pthread_mutex_init(&elfHelpLock, NULL);
	pthread_cond_init(santaCond, NULL);
	pthread_cond_init(deerCond, NULL);
	pthread_cond_init(elfCond, NULL);

	//Create santa
	pthread_create(&santa,NULL,producer,NULL);
	//Create elfs
	pthread_create(&e1,NULL,producer,NULL);
	pthread_create(&e2,NULL,producer,NULL);
	pthread_create(&e3,NULL,producer,NULL);
	pthread_create(&e4,NULL,producer,NULL);
	pthread_create(&e5,NULL,producer,NULL);
	pthread_create(&e6,NULL,producer,NULL);
	pthread_create(&e7,NULL,producer,NULL);
	pthread_create(&e8,NULL,producer,NULL);
	pthread_create(&e9,NULL,producer,NULL);
	pthread_create(&e10,NULL,producer,NULL);
	//create reindeer
	pthread_create(&r1,NULL,consumer,NULL);
	pthread_create(&r2,NULL,consumer,NULL);
	pthread_create(&r3,NULL,consumer,NULL);
	pthread_create(&r4,NULL,consumer,NULL);
	pthread_create(&r5,NULL,consumer,NULL);
	pthread_create(&r6,NULL,consumer,NULL);
	pthread_create(&r7,NULL,consumer,NULL);
	pthread_create(&r8,NULL,consumer,NULL);
	pthread_create(&r9,NULL,consumer,NULL);
	printf("hello World\n");
	return 0;
}
