//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"
#include "interrupts.h"
#include "resourceManage.h"

int main(int arg, char* argv[]) {

    int semVal = -55;

    //Initializations:

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);

    //alarm(10);

    initOssProcessManager();

    //Init semaphore
    sem_t* shmSemPtr = initShmSemaphore(SHM_KEY_SEM, shmSemSize, &shmSemID, SHM_OSS_FLAGS);
    if(sem_init(shmSemPtr, 1, 1) == -1) {
        perror("ERROR:sem_init failed");
        cleanupAll();
        exit(1);
    }

    //Init shared memory
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_OSS_FLAGS);
    initClock(shmClockPtr);

    shmClockPtr->nanoseconds = 5;

    res_desc_t* shmResourceDescPtr = (res_desc_t*)initSharedMemory(SHM_KEY_RESOURCE, shmResourceDescSize, &shmResourceDescID, SHM_OSS_FLAGS);
    
    (shmResourceDescPtr + 1)->currentAllocs[1000] = 5;
    
    int count = 0;
    while(1) {
        spawnProcess();

        sem_getvalue(shmSemPtr, &semVal);

        //Critical section
        sem_wait(shmSemPtr);
        printActiveProcessArray();
        printf("\t\t\t\t\toss hello #%d\n", count++);

        //Wait on dead child if there is one
        waitNoBlock();
        sem_post(shmSemPtr);

        //Check if a signal was received
        if(ossSignalReceivedFlag == 1) {
            killChildren();
            break;
        }
    }

    //Wait on remaining processes
    while(areActiveProcesses() == 1)
        waitNoBlock();

    //Cleanup
    destroyProcessManager();
    cleanupAll();
    
    

}
