//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef RES_MAN_H
#define RES_MAN_H

#include "shared.h"

#define MAX_QUEUE_SIZE 100

typedef struct queue_struct {
    int front;
    int back;
    Request array[MAX_QUEUE_SIZE];
} RequestQueue;

//Initialization
void initRequestArray();
void initResourceDescriptorArray();

//oss process resource functions:
void getRequests(Request* reqArray);
int isSafeState();
void allocRequests(Request* reqArray);

//Struct build/update functions


//usrPs process resource functions:
int makeRequest(Request* reqArray);

//Utility:
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);

#endif