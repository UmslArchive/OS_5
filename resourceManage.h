//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef RES_MAN_H
#define RES_MAN_H

#include "shared.h"

#define MAX_RESOURCES 10
#define MAX_RESOURCE_INSTANCES 10

//Matrices and vectors
extern int stateMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];
extern int allocMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];
extern int claimMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];
extern int resVec[MAX_RESOURCES];
extern int availVec[MAX_RESOURCES];

//UNPROCESSED - request sent but not yet received by oss.
//DENIED - request received and denied by oss. usr process will sleep.
//APPROVED - Request granted and allocd. Ready for new request.
//NULL_PROCESS - There is no associated active process in the process manager.
typedef enum request_state { 
    UNPROCESSED,
    DENIED,
    APPROVED,
    NULL_PROCESS,
    FINISHED,
} ReqState;

//One for each resource
typedef struct res_desc_struct {
    unsigned int maxAllocs;
    pid_t currentAllocs[MAX_RESOURCE_INSTANCES];
    int shareable;
} ResourceDescriptor;

//One for each process
typedef struct request_struct {
    ReqState reqState;
    pid_t pid;
    unsigned int maxClaims[MAX_RESOURCES];
    unsigned int resource;
    unsigned int amount;
    Clock timestamp;
    int isRelease;
} Request;

//Holds denied requests
typedef struct queue_struct {
    int front;
    int back;
    int numQueued;
    Request array[MAX_CHILD_PROCESSES];
} RequestQueue;

//Initialization
void initRequestArray(Request* reqArray);
void initResourceDescriptorArray(ResourceDescriptor* resArray);
void initMatrix(int matrix[MAX_CHILD_PROCESSES][MAX_RESOURCES]);
void initVector(int availVec[]);
void initMatricesAndVectors();

//oss process resource functions:
void updateClaimMatrix(Request* reqArray);
void updateAllocMatrix(ResourceDescriptor* resArray);
void updateAvailableVector(ResourceDescriptor* resArray);
void ossProcessRequests(Request* reqArray, ResourceDescriptor* resArray);
int isSafeState(Request* req);
void approveRequest(Request* requestIterator, ResourceDescriptor* resArray, pid_t pid);
void denyRequest(Request* requestIterator, pid_t pid);

//usr process resource functions:
void usrOnSpawnRequest(pid_t pid, Request* reqArray, ResourceDescriptor* descArray);
void usrSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount); //TODO

//Utility:
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);
ResourceDescriptor* getResourceDescriptorIterator(ResourceDescriptor* resArray, int resource);
void printRequest(Request* reqArray, pid_t pid);
void printResDesc(ResourceDescriptor* resArray, int resIndex);
void printAllRequests(Request* reqArray);
void printAllResDesc(ResourceDescriptor* resArray);
void printMatrix(FILE* fp, int mat[MAX_CHILD_PROCESSES][MAX_RESOURCES]);
void printVector(FILE* fp, int vec[], int size);
void copyMatrix(int src[MAX_CHILD_PROCESSES][MAX_RESOURCES], int dest[MAX_CHILD_PROCESSES][MAX_RESOURCES]);

#endif