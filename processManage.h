//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef PROC_MAN_H
#define PROC_MAN_H

#include "shared.h"

//oss process management functions:

//Function called by OSS. Spawns a child process and 
//stores its pid into the active process array
pid_t spawnProcess(int* activeProcesses);

//usrPs process management functions
void detach(int shmid);


#endif