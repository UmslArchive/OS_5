//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "shared.h"

//Keys
const key_t SHM_KEY_SEM = 0x66666666;   
const key_t SHM_KEY_CLOCK = 0x99999999;

//IDs
int shmSemID = 0;
int shmClockID = 0;

pid_t pid = 0;

sem_t* initShmSemaphore(key_t* key, size_t* size, int* shmid, int flags) {
    //Allocate shared memory and get an id
    *shmid = shmget(*key, *size, flags);
    if(*shmid < 0) {
        perror("ERROR:shmget failed(semaphore)");
        cleanupAll();
        exit(1);
    }

    //Assign pointer
    void* temp = shmat(*shmid, NULL, 0);
    if(temp == (void*) -1) {
        perror("ERROR:shmat failed(semaphore)");
        cleanupAll();
        exit(1);
    }

    //Init semaphore
    if(sem_init(temp, 1, 1) == -1) {
        perror("ERROR:sem_init failed");
        cleanupAll();
        exit(1);
    }

    return (sem_t*)temp;
}

void* initSharedMemory(key_t* key, size_t* size, int* shmid, int flags) {
    //Allocate shared memory and get an id
    *shmid = shmget(*key, *size, flags);
    if(*shmid < 0) {
        if(*key == SHM_KEY_CLOCK) {
            perror("ERROR:shmid failed(clock)");
        }
        cleanupAll();
        exit(10);
    }

    return NULL;
}

void detachAll() {
    if(shmClockID > 0)
        shmdt(&shmClockID);
    if(shmSemID > 0)
        shmdt(&shmSemID);

    fprintf(stderr, "Child PID %d detached\n", getpid());
}

void cleanupSharedMemory(int* shmid) {
    int cmd = IPC_RMID;
    int rtrn = shmctl(*shmid, cmd, NULL);
    if(rtrn == -1) {
        perror("ERROR:oss:shmctl failed");
        exit(1);
    }
}

void cleanupAll() {
    if(shmSemID > 0)
        cleanupSharedMemory(&shmSemID);

    if(shmClockID > 0)   
        cleanupSharedMemory(&shmClockID);
}

void advanceClock(Clock* mainClock, unsigned int sec, unsigned int nanosec) {
    //Subtract seconds off of nanoseconds if >= 1,000,000,000 
    while(nanosec >= 1000000000) {
        ++sec;
        nanosec -= 1000000000;
    }

    //Set the clock
    if(mainClock->nanoseconds + nanosec < 1000000000) {
        mainClock->nanoseconds += nanosec;
    }
    else {
        ++sec;
        mainClock->nanoseconds = nanosec - (1000000000 - mainClock->nanoseconds);
    }
    
    mainClock->seconds += sec;
}

Clock* initClock(){
    Clock* newClock = malloc(sizeof(Clock));
    newClock->nanoseconds = 0;
    newClock->seconds = 0;

    return newClock;
}

void printClock(Clock* clock) {
    if(clock != NULL)
        fprintf(stderr, "Clock Reads %d:%.9d\n", clock->seconds, clock->nanoseconds);
    else
        fprintf(stderr, "Clock is null\n");
}

void timeDifference(Clock* subtractFrom, Clock* subtractAmount) {
    
}