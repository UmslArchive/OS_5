//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"

//Just the alloc matrix updated with newest set of requests 
static int stateMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current resource allocs of all processes
static int allocMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current max claims of all processes
static int claimMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores max total resources available
static int resVec[MAX_RESOURCES];

//Stores current total available resources
static int availVec[MAX_RESOURCES];


/* typedef struct res_desc_struct {
    unsigned int maxAllocs;
    pid_t currentAllocs[MAX_RESOURCE_INSTANCES];
    int shareable;
} ResourceDescriptor;


typedef struct request_struct {
    unsigned int maxClaims;
    unsigned int resource;
    unsigned int amount;
    Clock timestamp;
} Request; */

void initRequestArray(Request* reqArray) {
    int i;
    Request* iterator = reqArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        iterator->maxClaims = 0;
        iterator->amount = 0;
        iterator->resource = 0;
        iterator->timestamp.nanoseconds = 0;
        iterator->timestamp.seconds = 0;
        iterator++;
    }
}

void initResourceDescriptorArray(ResourceDescriptor* resArray){
    int i, j;
    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        iterator->maxAllocs = 0;
        for(j = 0; j < MAX_RESOURCE_INSTANCES; ++j) {
            iterator->currentAllocs[j] = 0;
        }
        iterator->shareable = rand() % 2;
        iterator++;
    }
}

Request* getProcessRequestIterator(Request* reqArray, pid_t pid) {
    int index = getPidIndex(pid);

    if(index == -1) {
        fprintf(stderr, "Request iterator is NULL. PID not found in active array\n");
        return NULL;
    }

    Request* iterator = reqArray;

    int i;
    for(i = 0; i < index; ++i) {
        iterator++;
    }

    return iterator;
}

void getRequests(Request* reqArray) {
    //for each request place it into the SystemState
}

int isSafeState() {
    stateMat[0][0] = 5;
    return 0;
}

void allocRequests(Request* reqArray) {

}

//usrPs process resource functions:
int processSendRequest(Request* reqArray, pid_t pid, int resIndex) {
    return 0;
}

void printRequest(Request* reqArray, pid_t pid) {
     Request* iterator = getProcessRequestIterator(reqArray, pid);

     if(iterator != NULL) {
         fprintf(stderr, "ERROR: Couldn't print request. PID not in activePSArr\n");
         return;
     }
    
    fprintf("REQUEST from %d: mc=%d res=%d amt=%d at %d:%d",
        pid, 
        iterator->maxClaims, 
        iterator->resource, 
        iterator->amount, 
        iterator->timestamp.seconds,
        iterator->timestamp.nanoseconds);
}

void printResDesc(ResourceDescriptor* resArray, int resIndex) {

}

void printAllRequests(Request* reqArray) {

}

void printAllResDesc(ResourceDescriptor* resArray) {

}
