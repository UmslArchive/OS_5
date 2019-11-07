//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef RES_MAN_H
#define RES_MAN_H

#include "shared.h"

#define MAX_RESOURCES 20
#define MAX_RESOURCE_INSTANCES 10
#define MAX_QUEUE_SIZE 100

//1 for each resource
typedef struct res_desc_struct {
    unsigned int maxAllocs;
    pid_t currentAllocs[MAX_RESOURCE_INSTANCES];
    int shareable;
} ResourceDescriptor;

//1 for each process
typedef struct request_struct {
    unsigned int maxClaims;
    unsigned int resource;
    unsigned int amount;
    Clock timestamp;
} Request;

typedef struct queue_struct {
    int front;
    int back;
    Request array[MAX_QUEUE_SIZE];
} RequestQueue;

//Initialization
void initRequestArray(Request* reqArray);
void initResourceDescriptorArray(ResourceDescriptor* resArray);

//oss process resource functions:
void ossRetrieveRequests(Request* reqArray);
int isSafeState();
void allocRequests(Request* reqArray);

//usr process resource functions:
int usrProcessSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount);

//Utility:
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);
void printRequest(Request* reqArray, pid_t pid);
void printResDesc(ResourceDescriptor* resArray, int resIndex);
void printAllRequests(Request* reqArray);
void printAllResDesc(ResourceDescriptor* resArray);

#endif