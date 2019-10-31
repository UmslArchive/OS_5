//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "interrupts.h"

//Global signal flags
int ossSignalReceivedFlag = 0;
int usrSignalReceivedFlag = 0;

//Global sigaction structs
struct sigaction ossSigAction;
struct sigaction usrSigAction;

void ossSignalHandler(int signal) {
    switch(signal) {
        case SIGINT:
            fprintf(stderr, "\nOSS caught SIGINT signal\n");
            break;

        case SIGALRM:
            fprintf(stderr, "\nOSS caught SIGALRM signal\n");
            break;
    }    
    ossSignalReceivedFlag = 1;
}

void usrSignalHandler(int signal) {
    switch(signal) {
        case SIGTERM:
            fprintf(stderr, "\nUSR caught SIGTERM signal\n");
            fprintf(stderr, "Child PID %d detaching from shared memory...\n", getpid());
            break;
    }    
    usrSignalReceivedFlag = 1;
}

void ossInitSignalHandler() {
    memset(&ossSigAction, 0, sizeof(ossSigAction));
    ossSigAction.sa_handler = ossSignalHandler;
}

void usrInitSignalHandler() {
    memset(&usrSigAction, 0, sizeof(usrSigAction));
    usrSigAction.sa_handler = usrSignalHandler;
}