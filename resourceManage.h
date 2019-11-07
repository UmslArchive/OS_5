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

//UNPROCESSED - request sent but not yet received by oss.
//DENIED - request received and denied by oss. usr process will sleep.
//APPROVED - Request granted and alloc'd. Ready for new request.
//NULL_PROCESS - There is no associated active process in the process manager.
typedef enum request_state { 
    UNPROCESSED, 
    DENIED, 
    APPROVED, 
    NULL_PROCESS 
} ReqState;

//1 for each resource
typedef struct res_desc_struct {
    unsigned int maxAllocs;
    pid_t currentAllocs[MAX_CHILD_PROCESSES];
    int shareable;
} ResourceDescriptor;

//1 for each process
typedef struct request_struct {
    ReqState reqState;
    pid_t pid;
    unsigned int maxClaims;
    unsigned int resource;
    unsigned int amount;
    Clock timestamp;
    int isRelease;
} Request;

//Holds rejected requests
typedef struct queue_struct {
    int front;
    int back;
    Request array[MAX_CHILD_PROCESSES];
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