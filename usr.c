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

    printf("child %d execd\n", getpid());

    int count = 0;
    while(1) {
        if(count > 1000)
            break;

        //Check if a signal was received
        if(usrSignalReceivedFlag == 1)
            break;

        ++count;
    }

    sleep(1);

    fprintf(stderr, "DEATH %d\n", getpid());
    return 50;
}