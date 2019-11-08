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

void initMatrix(int matrix[MAX_CHILD_PROCESSES][MAX_RESOURCES]) {
    int i, j;

    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        for(j = 0; j < MAX_RESOURCES; ++j) {
            matrix[i][j] = 0;          
        }
    }
}

void initVector(int vector[]) {
    int i;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        vector[i] = 0;
    }
}

void initMatricesAndVectors() {
    initMatrix(stateMat);
    initMatrix(allocMat);
    initMatrix(claimMat);
    initVector(resVec);
    initVector(availVec);
}

void initRequestArray(Request* reqArray) {
    int i, j;
    Request* iterator = reqArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        iterator->reqState = NULL_PROCESS;
        iterator->pid = getPidOfIndex(i);

        for(j = 0; j < MAX_RESOURCES; ++j) {
            iterator->maxClaims[j] = 0;
        }

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

    if(reqIterator == NULL)
        return;
    
    //Randomly generate the amount max claim of a process for the each resource
    int claim, i;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        claim = rand() % (descIterator->maxAllocs + 1); //0 to 20
        reqIterator->maxClaims[i] = claim;
        descIterator++;
    }

    //Randomly generate which resource a process will like to have
    int resourceIndex;
    int valid = 0;
    while(valid == 0) {
        resourceIndex = rand() % MAX_RESOURCES;
        
        //Check if valid
        if(reqIterator->maxClaims[resourceIndex] != 0)
            valid = 1;
    }

    //Set claim correspondence
    claim = reqIterator->maxClaims[resourceIndex];

    reqIterator->pid = pid;
    reqIterator->resource = resourceIndex;
    reqIterator->isRelease = 0;
    reqIterator->amount = rand() % claim + 1; //1 to 20
    reqIterator->reqState = UNPROCESSED;
}

//Function places validated requests into the request array for a process.
//Valid meaning doesn't attempt anything impossible, but could still be an
//unsafe request. OSS will check for safety, not child process.
int usrProcessSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount) {
    Request* iterator = getProcessRequestIterator(reqArray, pid);

    return 0;
}

void updateClaimMatrix(Request* reqArray, int matrix[MAX_CHILD_PROCESSES][MAX_RESOURCES]) {
    Request* iterator = reqArray;

    if(reqArray == NULL) {
        fprintf(stderr, "ERROR: updateClaimMatrix : reqArray NULL\n");
        return;
    }

    int i, j;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        if(iterator->reqState != NULL_PROCESS) {
            for(j = 0; j < MAX_RESOURCES; ++j) {
                matrix[i][j] = iterator->maxClaims[j];
            }
        }
        iterator++;
    }
}

void printRequest(Request* reqArray, pid_t pid) {
    Request* iterator = getProcessRequestIterator(reqArray, pid);

    if(iterator == NULL) {
        fprintf(stderr, "ERROR: Couldn't print request. PID not in activePSArr\n");
        return;
    }

    fprintf(stderr, "REQUEST from %d: res=%d amt=%d at %d:%d",
        pid,
        iterator->resource, 
        iterator->amount, 
        iterator->timestamp.seconds,
        iterator->timestamp.nanoseconds);

    int i;
    fprintf(stderr, "\tMC: ");
    for(i = 0; i < MAX_RESOURCES; ++i) {
        fprintf(stderr, "%d(%d) ", i,  iterator->maxClaims[i]);
    }
    fprintf(stderr, "\n");
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

    fprintf(stderr, "RESOURCE #%d maxAllocs=%d shareable=%d", resIndex, iterator->maxAllocs, iterator->shareable);
    fprintf(stderr, "\tallocs -> ");
    for(i = 0; i < iterator->maxAllocs; ++i) {
        fprintf(stderr, "%d(%d) ", i, iterator->currentAllocs[i]);
    }
    fprintf(stderr, "\n");
}

void printAllRequests(Request* reqArray) {
    int i, j;
    Request* iterator = reqArray;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        fprintf(stderr, "[%.2d] REQUEST from %d: res=%d amt=%d at %d:%d",
        i,
        iterator->pid, 
        iterator->resource, 
        iterator->amount, 
        iterator->timestamp.seconds,
        iterator->timestamp.nanoseconds);

        fprintf(stderr, "\tMC: ");
        for(j = 0; j < MAX_RESOURCES; ++j) {
            fprintf(stderr, "%d(%d) ",j,  iterator->maxClaims[j]);
        }
        fprintf(stderr, "\n");

        iterator++;
    }
}

void printAllResDesc(ResourceDescriptor* resArray) {
    int i, j;
    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        fprintf(stderr, "RESOURCE #%d maxAllocs=%d shareable=%d", i, iterator->maxAllocs, iterator->shareable);
        fprintf(stderr, "\tallocs -> ");
        for(j = 0; j < iterator->maxAllocs; ++j) {
            fprintf(stderr, "%d(%d) ", j, iterator->currentAllocs[j]);
        }
        fprintf(stderr, "\n");
        iterator++;
    }
}

void printMatrix(FILE* fp, int mat[MAX_CHILD_PROCESSES][MAX_RESOURCES]) {
    int i, j;

    for(i = 0; i < MAX_RESOURCES; ++i) {
        fprintf(fp, "    R%.2d", i);
    }

    fprintf(fp, "\n");

    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        fprintf(fp, "P%.2d ", i);
        for(j = 0; j < MAX_RESOURCES; ++j) {
            fprintf(fp, "%.2d     ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
}