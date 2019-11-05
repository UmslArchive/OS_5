//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "interrupts.h"
#include "resourceManage.h"

typedef enum process_state { AWAKE, ASLEEP } pstate_t;

int main(int arg, char* argv[]) {

    //Initializations:

    srand(time(NULL));

    //Register signal handler
    usrInitSignalHandler();
    sigaction(SIGTERM, &usrSigAction, 0);
    sigaction(SIGINT, &usrSigAction, 0);

    //Init shared memory pointers
    sem_t* shmSemPtr = initShmSemaphore(SHM_KEY_SEM, shmSemSize, &shmSemID, SHM_USR_FLAGS);
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_USR_FLAGS);
    ResourceDescriptor* shmResourceDescPtr = (ResourceDescriptor*)initSharedMemory(SHM_KEY_RESOURCE, shmResourceDescSize, &shmResourceDescID, SHM_USR_FLAGS);
    Request* shmRequestPtr = (Request*)initSharedMemory(SHM_KEY_REQUEST, shmRequestSize, &shmRequestID, SHM_USR_FLAGS);

    //Save spawn time
    Clock spawnTime;
    spawnTime.seconds = shmClockPtr->seconds;
    spawnTime.nanoseconds = shmClockPtr->nanoseconds;

    //Generate first time limit
    Clock timeLimit;
    timeLimit.nanoseconds = spawnTime.nanoseconds;
    timeLimit.seconds = spawnTime.seconds;
    advanceClock(&timeLimit, 0, rand() % 499999999 + 1);

    int count = 0;
    while(1) {
        if(checkIfPassedTime(shmClockPtr, &timeLimit) == 1) {
            
            sem_wait(shmSemPtr);
                count++;

                //Print time
                /* fprintf(stderr, "\t\t\t\tUSR %d: ", getpid());
                printClock(shmClockPtr); */

                //Set new time limit
                advanceClock(&timeLimit, 0, rand() % 499999999 + 1);

                //Advance the main clock
                advanceClock(shmClockPtr, 0, 1);

            sem_post(shmSemPtr);
        }
        
        //Check if a signal was received
        if(usrSignalReceivedFlag == 1 || count > 20)
            break;
    }

    detachAll();
    //fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}
