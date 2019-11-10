//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "interrupts.h"
#include "resourceManage.h"

int main(int arg, char* argv[]) {

    //For piping the index of a forked process over to the child
    int fd[2];
    int pipeVal = 0;
    pipe(fd);
    
    //Initializations:
    
    srand(time(NULL));

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);
    sigaction(SIGTERM, &ossSigAction, 0);

    //Init Managers
    initOssProcessManager();
    initMatricesAndVectors();

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
    initRequestArray(shmRequestPtr);
    initResourceDescriptorArray(shmResourceDescPtr);

    //Generate first random process spawn time
    Clock spawnTime;
    spawnTime.nanoseconds = rand() % 499999999 + 1;
    spawnTime.seconds = 0;
    
    int numProcesses = 0;

    //DEBUG spawn only one process    
    while(1) {

        updateAvailableVector(shmResourceDescPtr);
        
        if(checkIfPassedTime(shmClockPtr, &spawnTime) == 1) {
            //Spawn
            if(spawnProcess() == 1) {
                fprintf(stderr, "%d Process Spawned:\n", ++numProcesses);
                printClock(shmClockPtr);
            }

            //Set new time limit
            setClock(&spawnTime, shmClockPtr->seconds, shmClockPtr->nanoseconds);
            advanceClock(&spawnTime, 0, rand() % 499999999 + 1);
        }

        //Critical section
        sem_wait(shmSemPtr);
            advanceClock(shmClockPtr, 0, 10000);
            /* fprintf(stderr, "OSS : ");
            printClock(shmClockPtr); */
        
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
