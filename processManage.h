//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef PROC_MAN_H
#define PROC_MAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>

#define MAX_CHILD_PROCESSES 18

/* extern pid_t pid;
extern int exitStatus;
extern pid_t* activeProcesses; */

//Initialization/deallocation
int initOssProcessManager();
int destroyProcessManager();

//oss process management functions:
int spawnProcess();
void waitNoBlock();
int areActiveProcesses();
void killChildren();

//Utility
void printActiveProcessArray();
int getIndexOfPid(pid_t searchPid);
pid_t getPidOfIndex(int index);


#endif