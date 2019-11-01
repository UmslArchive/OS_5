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

    //Initializations:

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);

    initOssProcessManager();

    while(1) {
        spawnProcess();

        //Check if a signal was received
        if(ossSignalReceivedFlag == 1)
            break;
    }

    //Wait on remaining processes
    waitWhileStillActiveProcesses();

    //Cleanup
    destroyProcessManager();
    
    

}
