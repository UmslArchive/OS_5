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
    memset(&ossSigAction, 0, sizeof(ossSigAction));
}

void usrSignalHandler(int signal) {

}

void ossInitSignalHandlers() {

}

void usrInitSignalHandlers() {

}