//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef RES_MAN_H
#define RES_MAN_H

#include "shared.h"

typedef struct system_state_struct {
    //from book
} SystemState;

//oss process resource functions
void getRequests(Request* reqArray);
int isSafeState(SystemState* whatIfState);
void allocRequests(Request* reqArray);

//usrPs process resource functions
int makeRequest(Request* reqArray);

//Utility
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);

#endif