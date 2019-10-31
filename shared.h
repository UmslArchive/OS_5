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

//Keys
#define SHM_KEY_SEM         0x66666666
#define SHM_KEY_PCB_ARRAY   0x77777777
#define SHM_KEY_MSG         0x88888888
#define SHM_KEY_CLOCK       0x99999999

//----------------------------------------

typedef enum { RAN, READY, DISPATCHED } ProcessState;

//Structs
typedef struct clock_struct {
    unsigned int seconds;
    unsigned int nanoseconds;
} Clock;

//Shared memory struct utility function prototypes
Clock* initClock();
void advanceClock(Clock* mainClock, unsigned int sec, unsigned int nanosec);
void timeDifference(Clock* subtractFrom, Clock* subtractAmount);
void printClock(Clock* clock);

#endif