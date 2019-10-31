//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#define _XOPEN_SOURCE //Needed for sigaction struct

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern int ossSignalReceived;
extern int usrSignalReceived;

extern struct sigaction ossSigAction;
extern struct sigaction usrSigAction;

void ossInitSignalHandlers();
void usrInitSignalHandlers();

void ossSignalHandler(int signal);
void usrSignalHandler(int signal);

#endif