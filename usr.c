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

    printf("child %d execd\n", getpid());

    /* while(1) {

        sem_wait(sem);
        printf("child hello\n");
        sem_post(sem);

        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;
    } */

    //sem_close(sem);
    return 50;
}