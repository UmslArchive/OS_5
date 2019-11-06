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
void initRequestArray(Request* reqArray);
void initResourceDescriptorArray(ResourceDescriptor* resArray);

//oss process resource functions:
void getRequests(Request* reqArray);
int isSafeState();
void allocRequests(Request* reqArray);

//usrPs process resource functions:
int processSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount);

//Utility:
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);
void printRequest(Request* reqArray, pid_t pid);
void printResDesc(ResourceDescriptor* resArray, int resIndex);
void printAllRequests(Request* reqArray);
void printAllResDesc(ResourceDescriptor* resArray);

#endif