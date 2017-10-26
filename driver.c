#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define minimum_number 100
#define max_number 2000

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

//Function prototypes

void *elf();
void *santaGuy();
void *reindeer();
void deliverPresents();
void getHitched();
void prepSleigh();
void helpElves();
void getHelp();


int main(){
	printf("Parent beginning\n");
	pthread_t santa, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, 
				r1, r2, r3, r4, r5, r6, r7, r8, r9;	
	
	//init lock and CVs
	pthread_mutex_init(&counterLock, NULL);
	pthread_mutex_init(&elfHelpLock, NULL);
	pthread_cond_init(&santaCond, NULL);
	pthread_cond_init(&deerCond, NULL);
	pthread_cond_init(&elfCond, NULL);

	//Create santa
	pthread_create(&santa,NULL,santaGuy,NULL);
	//Create elfs
	//pthread_create(&e1,NULL,elf,NULL);
	//pthread_create(&e2,NULL,elf,NULL);
	//pthread_create(&e3,NULL,elf,NULL);
	//pthread_create(&e4,NULL,elf,NULL);
	//pthread_create(&e5,NULL,elf,NULL);
	//pthread_create(&e6,NULL,elf,NULL);
	//pthread_create(&e7,NULL,elf,NULL);
	//pthread_create(&e8,NULL,elf,NULL);
	//pthread_create(&e9,NULL,elf,NULL);
	//pthread_create(&e10,NULL,elf,NULL);
	//create reindeer
	pthread_create(&r1,NULL,reindeer,NULL);
	pthread_create(&r2,NULL,reindeer,NULL);
	pthread_create(&r3,NULL,reindeer,NULL);
	pthread_create(&r4,NULL,reindeer,NULL);
	pthread_create(&r5,NULL,reindeer,NULL);
	pthread_create(&r6,NULL,reindeer,NULL);
	pthread_create(&r7,NULL,reindeer,NULL);
	pthread_create(&r8,NULL,reindeer,NULL);
	pthread_create(&r9,NULL,reindeer,NULL);

	while(1){
		//run this until I quit the program
	}
	
	printf("Program quit. Parent ended\n");
	return 0;
}

void deliverPresents(){
	printf("Santa finished delivering Presents\n");
}

void getHitched(){
	printf("Reindeer Hitched\n");
}

void prepSleigh(){
	printf("Santa is prepping the sleigh\n");
}

void helpElves(){
	printf("Santa helped an elf\n");
}

void getHelp(){
	printf("Elf getting help\n");
}

void *elf(){
	while(1){
		
		pthread_mutex_lock(&elfHelpLock);
		pthread_mutex_lock(&counterLock);
		elfCounter++;
		if(elfCounter == 3){
			pthread_cond_signal(&santaCond);
		}else{
			pthread_mutex_unlock(&elfHelpLock);
		}
		
		printf("elf waiting\n");
		//while (elfCounter != 3)
			pthread_cond_wait(&elfCond, &counterLock);

		getHelp();
		
		//pthread_mutex_lock(&counterLock);
			elfCounter--;
			if(elfCounter == 0){
				pthread_mutex_unlock(&elfHelpLock);
			}
				
		pthread_mutex_unlock(&counterLock);
		
	}
}

void *reindeer(){
	while(1){
		// get counter lock
		pthread_mutex_lock(&counterLock);
		// increase the number of reindeer back from vacation
		deerCounter++;
		// check if they're all back
		if(deerCounter == 9){
			// wake up santa
			printf("all reindeer back, time to wake up santa\n");
			pthread_cond_signal(&santaCond);
		}
		
		pthread_cond_wait(&deerCond, &counterLock);
	
		getHitched();
	
		// release the counter lock
		pthread_mutex_unlock(&counterLock);
	}
}

void *santaGuy(){
	while(1){
		pthread_cond_wait(&santaCond, &counterLock);
		printf("Santa woke up\n");
		if(deerCounter == 9){
			deerCounter = 0;
			prepSleigh();
			pthread_cond_broadcast(&deerCond);
		}else{
			pthread_cond_broadcast(&elfCond);
			helpElves();
		}
		pthread_mutex_unlock(&counterLock);
	}
}
