//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"

//Shared memory struct array sizes
const size_t shmResourceDescSize = MAX_RESOURCES * sizeof(ResourceDescriptor);
const size_t shmRequestSize = MAX_CHILD_PROCESSES * sizeof(Request);

//Stores alloc matrix updated with newest set of requests (hypothetical future state)
static int stateMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current resource allocs of all processes
static int allocMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current max claims of all processes
static int claimMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores max total resources available
static int resVec[MAX_RESOURCES];

//Stores current total available resources
static int availVec[MAX_RESOURCES];

void initRequestArray(Request* reqArray) {
    int i;
    Request* iterator = reqArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        iterator->reqState = NULL_PROCESS;
        iterator->pid = getPidOfIndex(i);
        iterator->maxClaims = 0;
        iterator->amount = 0;
        iterator->resource = 0;
        iterator->timestamp.nanoseconds = 0;
        iterator->timestamp.seconds = 0;
        iterator->isRelease = 0;
        iterator++;
    }
}

void initResourceDescriptorArray(ResourceDescriptor* resArray){
    int i, j;
    int isShareable;

    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        iterator->maxAllocs = rand() % 10 + 1;

        for(j = 0; j < MAX_RESOURCE_INSTANCES; ++j) {
            iterator->currentAllocs[j] = 0;
        }

        //20% of resources are shareable
        isShareable = rand() % 100;
        if(isShareable < 20) isShareable = 1; else isShareable = 0;
        iterator->shareable = isShareable;

        iterator++;
    }
}

Request* getProcessRequestIterator(Request* reqArray, pid_t pid) {
    int index = getIndexOfPid(pid);

    if(index == -1) {
        fprintf(stderr, "ERROR: Request iterator is NULL\n");
        return NULL;
    }

    if(reqArray == NULL) {
        fprintf(stderr, "ERROR: PID not found in active array\n");
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
    return 0;
}

void allocRequests(Request* reqArray) {

}

//usrPs process resource functions:

void usrOnSpawnRequest(pid_t pid, Request* reqArray, ResourceDescriptor* descArray) {
    Request* reqIterator = getProcessRequestIterator(reqArray, pid);
    ResourceDescriptor* descIterator = descArray;

    //Randomly generate which resource a process will like to have
    int resourceIndex = rand() % MAX_RESOURCES;

    //Set the resource description iterator
    int i;
    for(i = 0; i < resourceIndex; ++i) {
        descIterator++;
    }
    
    //Randomly generate the amount max claim of a process for the generated resource
    int claim = rand() % descIterator->maxAllocs + 1;

    //Set the proccesses request array
    reqIterator->maxClaims = claim;
    reqIterator->pid = pid;
    reqIterator->resource = resourceIndex;
    reqIterator->isRelease = 0;
    reqIterator->reqState = UNPROCESSED;
    reqIterator->amount = rand() % claim + 1;
}

//Function places validated requests into the request array for a process.
//Valid meaning doesn't attempt anything impossible, but could still be an
//unsafe request. OSS will check for safety, not child process.
int usrProcessSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount) {
    Request* iterator = getProcessRequestIterator(reqArray, pid);

    return 0;
}

void printRequest(Request* reqArray, pid_t pid) {
     Request* iterator = getProcessRequestIterator(reqArray, pid);

     if(iterator == NULL) {
         fprintf(stderr, "ERROR: Couldn't print request. PID not in activePSArr\n");
         return;
     }
    
    fprintf(stderr, "REQUEST from %d: mc=%d res=%d amt=%d at %d:%d\n",
        pid, 
        iterator->maxClaims, 
        iterator->resource, 
        iterator->amount, 
        iterator->timestamp.seconds,
        iterator->timestamp.nanoseconds);
}

void printResDesc(ResourceDescriptor* resArray, int resIndex) {
    if(resArray == NULL || resIndex < 0 || resIndex >= MAX_RESOURCES) {
        fprintf(stderr, "ERROR: couldn't print resource description -- Invalid params\n");
        return;
    }

    int i;
    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < resIndex; ++i) {
        iterator++;
    }

    fprintf(stderr, "RESOURCE #%d maxAllocs=%d shareable=%d\n", resIndex, iterator->maxAllocs, iterator->shareable);
    fprintf(stderr, "\tallocs -> ");
    for(i = 0; i < iterator->maxAllocs; ++i) {
        fprintf(stderr, "%d(%d) ", i, iterator->currentAllocs[i]);
    }
    fprintf(stderr, "\n");
}

void printAllRequests(Request* reqArray) {
    int i;
    Request* iterator = reqArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        fprintf(stderr, "REQUEST from %d: mc=%d res=%d amt=%d at %d:%d\n",
        iterator->pid, 
        iterator->maxClaims, 
        iterator->resource, 
        iterator->amount, 
        iterator->timestamp.seconds,
        iterator->timestamp.nanoseconds);

        iterator++;
    }
}

void printAllResDesc(ResourceDescriptor* resArray) {
    int i, j;
    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        fprintf(stderr, "RESOURCE #%d maxAllocs=%d shareable=%d\n", i, iterator->maxAllocs, iterator->shareable);
        fprintf(stderr, "\tallocs -> ");
        for(j = 0; j < iterator->maxAllocs; ++j) {
            fprintf(stderr, "%d(%d) ", j, iterator->currentAllocs[j]);
        }
        fprintf(stderr, "\n");
        iterator++;
    }
}