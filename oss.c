//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"
#include "processManage.h"
#include "interrupts.h"

int main(int arg, char* argv[]) {

    //Initializations:

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);

    initOssProcessManager();

    spawnProcess();
    spawnProcess();

    waitWhileStillActiveProcesses();

    /* while(1) {
        //Check if a signal was received
        sem_wait(sem);
        sleep(1);
        printf("oss hello\n");
        sem_post(sem);

        if(ossSignalReceivedFlag == 1)
            break;
    } */

    //Cleanup
    //sem_close(sem);
    destroyProcessManager();
    
    

}
