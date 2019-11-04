//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "shared.h"

#define MAX_RESOURCES 20

//Keys
const key_t SHM_KEY_SEM = 0x66666666;   
const key_t SHM_KEY_CLOCK = 0x99999999;
const key_t SHM_KEY_RESOURCE = 0x77777777;
const key_t SHM_KEY_REQUEST = 0x88888888;


//IDs
int shmSemID = 0;
int shmClockID = 0;
int shmResourceDescID = 0;
int shmRequestID = 0;


//Sizes
const size_t shmSemSize = sizeof(sem_t);
const size_t shmClockSize = sizeof(Clock);
const size_t shmResourceDescSize = MAX_RESOURCES * sizeof(ResourceDescriptor);
const size_t shmRequestSize = MAX_CHILD_PROCESSES * sizeof(Request);

const int SHM_OSS_FLAGS = IPC_CREAT | IPC_EXCL | 0777;
const int SHM_USR_FLAGS = 0777;

sem_t* initShmSemaphore(const key_t key, const size_t size, int* shmid, int flags) {
    //Allocate shared memory and get an id
    *shmid = shmget(key, size, flags);
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

    /* //Init semaphore
    if(sem_init((sem_t*)temp, 1, 1) == -1) {
        perror("ERROR:sem_init failed");
        cleanupAll();
        exit(1);
    } */

    return (sem_t*)temp;
}

void* initSharedMemory(const key_t key, const size_t size, int* shmid, int flags) {
    //Allocate shared memory and get an id
    *shmid = shmget(key, size, flags);
    if(*shmid < 0) {
        if(key == SHM_KEY_CLOCK) {
            perror("ERROR:shmid failed(clock)");
        }
        cleanupAll();
        exit(10);
    }

    //Assign pointer
    void* temp = shmat(*shmid, NULL, 0);
    if(temp == (void*) -1) {
        if(key == SHM_KEY_CLOCK) { 
            perror("ERROR:oss:shmat failed(clock)");
        }
        cleanupAll();
        exit(20);
    }

    return temp;
}

void detachAll() {
    if(shmClockID > 0)
        shmdt(&shmClockID);
    if(shmSemID > 0)
        shmdt(&shmSemID);
    if(shmRequestID > 0)
        shmdt(&shmRequestID);
    if(shmResourceDescID > 0)
        shmdt(&shmResourceDescID);
    

    //fprintf(stderr, "Child PID %d detached\n", getpid());
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

    if(shmResourceDescID > 0)
        cleanupSharedMemory(&shmResourceDescID);

    if(shmRequestID > 0)
        cleanupSharedMemory(&shmRequestID);
}

//Clock functions:

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

void initClock(Clock* clock){
    clock->seconds = 0;
    clock->nanoseconds = 0;
}

void printClock(Clock* clock) {
    if(clock != NULL)
        fprintf(stderr, "Clock Reads %d:%.9d\n", clock->seconds, clock->nanoseconds);
    else
        fprintf(stderr, "Clock is null\n");
}

Clock timeDifference(Clock* minuend, Clock* subtrahend) {
    int secondsDifference = 0;
    int nanoDifference = 0;

    secondsDifference = minuend->seconds - subtrahend->seconds;
    nanoDifference = minuend->nanoseconds - subtrahend->nanoseconds;

    if(nanoDifference < 0) {
        --secondsDifference;
        nanoDifference = 1000000000 + nanoDifference;
    }

    Clock rtrnClock;
    rtrnClock.nanoseconds = nanoDifference;
    rtrnClock.seconds = secondsDifference;

    return rtrnClock;
}

int checkIfPassedTime(Clock* mainClock, Clock* timeLimit) {
    if(mainClock->seconds > timeLimit->seconds)
        return 1;

    if(mainClock->seconds == timeLimit->seconds && mainClock->nanoseconds >= timeLimit->nanoseconds)
        return 1;

    return 0;
}