#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define minimum_number 100
#define max_number 2000

/*
	Matthew Likwarz

	There are 4 locks and 3 CVs in this program. Two of the locks are to lock out the reindeer
	counter and elf counter. I probably could have made it work with one, but it works with two 
	so I'm going to keep it. Then there is a elfHelpLock which will keep other elfs from 
	accessing the elfCounter while 3 elfs are getting help from santa. The last lock is the 
	santaSleepLock. This lock is what santa holds onto while he is awake.

	There are 3 CVs in here. The purpose of all of them is the same, but there is a specific one
	for santa, elfs, and reindeer. The purpose of all of these CVs is to be a designated waiting
	area for each of them. 

	Other than the locks and CVs there are two counters. One for the elfs and the other for the 
	reindeer. Also inside the elf method there is a bool val that is meant to signal which elf
	is holding the elfHelpLock. That way I know to have that elf wait to be the last one so it
	can be the one who unlocks it.

*/

//counters
int elfCounter = 0;
int deerCounter = 0;

//locks
pthread_mutex_t elfCounterLock; // Locks the elf counter
pthread_mutex_t deerCounterLock; // Locks deer counter
pthread_mutex_t elfHelpLock; // keeps other elfs out while 3 are being helped by santa
pthread_mutex_t santaSleepLock; // this is the lock that santa grabs once he wakes up

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
	pthread_mutex_init(&elfCounterLock, NULL);
	pthread_mutex_init(&deerCounterLock, NULL);
	pthread_mutex_init(&elfHelpLock, NULL);
	pthread_mutex_init(&santaSleepLock, NULL);
	pthread_cond_init(&santaCond, NULL);
	pthread_cond_init(&deerCond, NULL);
	pthread_cond_init(&elfCond, NULL);

	//Create santa
	pthread_create(&santa,NULL,santaGuy,NULL);
	//Create elfs
	pthread_create(&e1,NULL,elf,NULL);
	pthread_create(&e2,NULL,elf,NULL);
	pthread_create(&e3,NULL,elf,NULL);
	pthread_create(&e4,NULL,elf,NULL);
	pthread_create(&e5,NULL,elf,NULL);
	pthread_create(&e6,NULL,elf,NULL);
	pthread_create(&e7,NULL,elf,NULL);
	pthread_create(&e8,NULL,elf,NULL);
	pthread_create(&e9,NULL,elf,NULL);
	pthread_create(&e10,NULL,elf,NULL);
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
	
	return 0;
}

void deliverPresents(){
	printf("Santa finished delivering Presents\n");
}

void getHitched(){
	printf("Reindeer Hitched\n");
}

void prepSleigh(){
	printf("Santa is prepping the sleigh and delivering presents\n");
}

void helpElves(){
	printf("Santa helped an elf\n");
}

void getHelp(){
	printf("Elf %i getting help\n", elfCounter);
}

// There are two locks here. The first lock grabed make it so only one elf at a time
// can go to santa for help. This lock also makes the others wait while 3 elfs are getting help
// The other lock makes it so only one elf at a time can increment the elfCounter.
// There is one elf CV here where the elfs will wait until there are 3. Then they will signal
// santa.
void *elf(){
	while(1){
		printf("Making toys\n");
		int hasHelpLock = 0;
		// compute a random time between 100 and 2000
		int random = rand() % (max_number + 1 - minimum_number) + minimum_number;
		// sleep for that time in ms. That is why I multiply by 1000
		usleep(random * 1000);
		// grab the elf help lock
		printf("need help from santa\n");
		pthread_mutex_lock(&elfHelpLock);
		// lock out the counter
		pthread_mutex_lock(&elfCounterLock);
		// inc the elf counter 
		elfCounter = elfCounter + 1;
		// check if there are now three elves waiting.
		if(elfCounter == 3){
			hasHelpLock = 1;
			pthread_cond_signal(&santaCond);
		}else{
			pthread_mutex_unlock(&elfHelpLock);
		}
		
		printf("elf %i waiting\n", elfCounter);
		pthread_cond_wait(&elfCond, &elfCounterLock);
		
		// make the guy who has the lock wait to be the last one so he can unlock the mutex
		if(hasHelpLock == 1){
			printf("The elf who has the lock is waiting to be the last one\n");
			while(elfCounter > 1){
						
			}
		}

		getHelp();
		
		elfCounter--;
		if(elfCounter == 0){
			pthread_mutex_unlock(&elfHelpLock);
		}
		printf("elf going back to work\n");
		pthread_mutex_unlock(&elfCounterLock);
	}
}

// There is one lock being used here. That lock makes it so only one reindeer can increment
// the counter at a time. Once all the reindeer are back the last one will signal santa.
// There is one CV here where the reindeer will wait until the last one is back.
void *reindeer(){
	while(1){
		printf("Reindeer on vacation\n");
		int random = rand() % (max_number + 1 - minimum_number) + minimum_number;
		usleep(random * 1000);
		// get counter lock
		pthread_mutex_lock(&deerCounterLock);
		// increase the number of reindeer back from vacation
		deerCounter++;
		// check if they're all back
		if(deerCounter == 9){
			// wake up santa
			printf("all reindeer back, time to wake up santa\n");
			pthread_cond_signal(&santaCond);
		}
		printf("waiting in the warming hut\n");
		pthread_cond_wait(&deerCond, &deerCounterLock);
		
		getHitched();
	
		// release the counter lock
		pthread_mutex_unlock(&deerCounterLock);
	}
}

// There are two locks in here. I don't think I need both of them, but it works as far as I can
// tell so I'm going to leave it. The first lock is the one santa grabs once he is woken up. 
// The next lock grabbed is the deerCounterLock, because santa will reset that to 0 before 
// delivering presents. Other than that there is a broadcast on the reindeer CV and one on the
// elf CV
void *santaGuy(){
	while(1){
		pthread_mutex_lock(&santaSleepLock);
		pthread_cond_wait(&santaCond, &santaSleepLock);
		printf("Santa woke up\n");
		if(deerCounter == 9){
			pthread_mutex_lock(&deerCounterLock);
			deerCounter = 0;
			prepSleigh();
			pthread_cond_broadcast(&deerCond);
			pthread_mutex_unlock(&deerCounterLock);
			deliverPresents();
		}else{
			helpElves();
			pthread_cond_broadcast(&elfCond);
		}
		pthread_mutex_unlock(&santaSleepLock);
		printf("Santa back asleep\n");
	}
}
