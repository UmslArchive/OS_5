//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"
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

    //Generate first time limit
    Clock timeLimit;
    timeLimit.nanoseconds = rand() % 499999999 + 1;
    timeLimit.seconds = 0;
    
    while(1) {

        if(checkIfPassedTime(shmClockPtr, &timeLimit) == 1) {
            sem_wait(shmSemPtr);
                //Set new time limit
                setClock(&timeLimit, shmClockPtr->seconds, shmClockPtr->nanoseconds);
                advanceClock(&timeLimit, 0, rand() % 499999999 + 1);
                advanceClock(shmClockPtr, 1, 0);
                fprintf(stderr, "\t\t\t\tUSR : ");
                printClock(shmClockPtr);
            sem_post(shmSemPtr);
        }
        

        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;
    }

    detachAll();
    //fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}
