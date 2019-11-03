//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

//Keys
extern const key_t SHM_KEY_SEM;     
extern const key_t SHM_KEY_CLOCK;

//IDs
extern int shmSemID;
extern int shmClockID;

//Sizes
extern const size_t shmSemSize;
extern const size_t shmClockSize;

//Oss Flags
extern const int SHM_OSS_FLAGS;
extern const int SHM_USR_FLAGS;

extern pid_t pid;

//----------------------------------------

//Structs
typedef struct clock_struct {
    unsigned int seconds;
    unsigned int nanoseconds;
} Clock;

//Shared mem init functions
sem_t* initShmSemaphore(const key_t key, const size_t size, int* shmid, int flags);
void* initSharedMemory(const key_t key, const size_t size, int* shmid, int flags);

//Cleanup functions
void detachAll();
void cleanupSharedMemory(int* shmid);
void cleanupAll();

//Clock functions
void initClock(Clock* clock);
void advanceClock(Clock* mainClock, unsigned int sec, unsigned int nanosec);
void timeDifference(Clock* subtractFrom, Clock* subtractAmount);
void printClock(Clock* clock);

#endif