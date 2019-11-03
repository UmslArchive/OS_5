//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"
#include "processManage.h"
#include "interrupts.h"
#include "shared.h"

int main(int arg, char* argv[]) {

    //Register signal handler
    usrInitSignalHandler();
    sigaction(SIGTERM, &usrSigAction, 0);
    sigaction(SIGINT, &usrSigAction, 0);

    //Init shared memory
    sem_t* shmSemPtr = initShmSemaphore(SHM_KEY_SEM, shmSemSize, &shmSemID, SHM_USR_FLAGS);
    Clock* shmClockPtr = (Clock*)initSharedMemory(SHM_KEY_CLOCK, shmClockSize, &shmClockID, SHM_USR_FLAGS);
    
    int count = 0;
    while(1) {
        sleep(1);
        //Critical section
        sem_wait(shmSemPtr);
        printf("usr %d hello #%d\n", getpid(), count++);
        sem_post(shmSemPtr);

        if(count > 20)
            break;

        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;
    }

    detachAll();

    fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}