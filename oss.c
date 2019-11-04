//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"
#include "interrupts.h"
#include "resourceManage.h"

int main(int arg, char* argv[]) {

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

    //Init shared memory pointers
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_OSS_FLAGS);
    ResourceDescriptor* shmResourceDescPtr = (ResourceDescriptor*)initSharedMemory(SHM_KEY_RESOURCE, shmResourceDescSize, &shmResourceDescID, SHM_OSS_FLAGS);
    Request* shmRequestPtr = (Request*)initSharedMemory(SHM_KEY_REQUEST, shmRequestSize, &shmRequestID, SHM_OSS_FLAGS);

    //Init shared memory values
    initClock(shmClockPtr);
    
    while(1) {

        spawnProcess();

        //Critical section
        sem_wait(shmSemPtr);
        sem_post(shmSemPtr);

        //Wait on dead child if there is one
        waitNoBlock();

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
