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

    alarm(5);

    initOssProcessManager();

    while(1) {
        spawnProcess();

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
    
    

}
