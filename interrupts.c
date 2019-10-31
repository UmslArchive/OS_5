//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "interrupts.h"

//Global signal received indicator flags
int ossSignalReceived = 0;
int usrSignalReceived = 0;

//Signal 
struct sigaction ossSigAction;
struct sigaction usrSigAction;

void ossSignalHandler(int signal) {
    switch(signal) {
        case SIGINT:
            fprintf(stderr, "\nOSS caught SIGINT signal\n");
            break;
    }    
    ossSignalReceived = 1;
}

void usrSignalHandler(int signal) {
    switch(signal) {
        case SIGTERM:
            fprintf(stderr, "\nUSR caught SIGTERM signal\n");
            fprintf(stderr, "Child PID %d detaching from shared memory...\n", getpid());
            break;
    }    
    usrSignalReceived = 1;
}

void ossInitSignalHandlers() {
    memset(&ossSigAction, 0, sizeof(ossSigAction));
    ossSigAction.sa_handler = ossSignalHandler;
}

void usrInitSignalHandlers() {
    memset(&usrSigAction, 0, sizeof(usrSigAction));
    usrSigAction.sa_handler = usrSignalHandler;
}