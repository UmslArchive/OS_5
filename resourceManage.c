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
int stateMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current resource allocs of all processes
int allocMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores current max claims of all processes
int claimMat[MAX_CHILD_PROCESSES][MAX_RESOURCES];

//Stores max total resources available
int resVec[MAX_RESOURCES];

//Stores current total available resources
int availVec[MAX_RESOURCES];

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
        resVec[i] = iterator->maxAllocs;

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

ResourceDescriptor* getResourceDescriptorIterator(ResourceDescriptor* resArray, int resource) {
    if(resource < 0 || resource >= MAX_RESOURCES) {
        fprintf(stderr, "ERROR: Invalid resource selected. Returning NULL iterator.\n");
        return NULL;
    }

    int i;
    ResourceDescriptor* iterator = resArray;
    for(i = 0; i < resource; ++i) {
        iterator++;
    }

    return iterator;
}

int isSafeState(Request* req) {
    int i, j;
    
    //Vector to contain processes which have already been determined possible to clear
    int beenProcessed[MAX_CHILD_PROCESSES];

    //Vector to contain hypothetical availabilities
    int hypoAvailVec[MAX_RESOURCES];

    //Init beenProcessed
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        beenProcessed[i] = 0;
    }

    //Set hypoAvailVec = resVec
    for(i = 0; i < MAX_RESOURCES; ++i) {
        hypoAvailVec[i] = resVec[i];
    }

    //Subtract away "allocd" resources
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        for(j = 0; j < MAX_RESOURCES; ++j) {
            hypoAvailVec[j] -= stateMat[i][j];
        }
    }

    //Check if possible
    if(req->amount > hypoAvailVec[req->resource]) {
        return 0;
    }

    //Check if a path to completion exists
    int zerod;
    int full;
    int possible = 1;
    while(possible == 1) {
        zerod = 1;
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(beenProcessed[i] == 0) {
                for(j = 0; j < MAX_RESOURCES; ++j) {
                    //if need more than available
                    if(claimMat[i][j] - stateMat[i][j] > hypoAvailVec[j]) {
                        zerod = 0;
                        break;
                    }
                }

                if(zerod == 1) {
                    beenProcessed[i] = 1;

                    //Add back to hypo avail vec
                    for(j = 0; j < MAX_RESOURCES; ++j) {
                        hypoAvailVec[j] += claimMat[i][j];
                        stateMat[i][j] = 0;
                    }

                    break;
                }

                if(i != MAX_CHILD_PROCESSES - 1)
                    zerod = 1;

            }
        }

        //check for full
        full = 1;
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(beenProcessed[i] == 0) {
                full = 0;
                break;
            }
        }

        if(zerod == 0 && full == 0)
            return 0;

        if(full == 1)
           return 1;
    }
}

void approveRequest(Request* requestIterator, ResourceDescriptor* resArray, pid_t pid){
    ResourceDescriptor* resIterator = resArray;
    int allocAmount = requestIterator->amount;
    int i, j;
    int count = 0;

    //Move resource descriptor iterator to correct box
    for(i = 0; i < requestIterator->resource; ++i) {
        resIterator++;
    }

    printf("[%d]PROCESSING REQUEST pid = %d, res = %d, amount = %d\n",getIndexOfPid(pid), pid, requestIterator->resource, allocAmount);

    //While there are still resources to alloc
    while(allocAmount > 0) {
        //sleep(1);
        //Find first empty slot in currentAllocs of resource descriptor
        for(j = 0; j < resIterator->maxAllocs; ++j) {
            if(resIterator->currentAllocs[j] == 0) {
                //Put pid into allocation slot
                resIterator->currentAllocs[j] = pid;
                --allocAmount;
                count = 0;
                break;
            }
        }
        count++;
        if(count > 11)
            exit(0);

        fprintf(stderr, "\t");
        printResDesc(resArray, i);
    }
}

void denyRequest(Request* requestIterator, pid_t pid) {

}

//usrPs process resource functions:

void usrOnSpawnRequest(pid_t pid, Request* reqArray, ResourceDescriptor* descArray) {
    Request* reqIterator = getProcessRequestIterator(reqArray, pid);
    ResourceDescriptor* descIterator = descArray;

    if(reqIterator == NULL || descArray == NULL)
        return;
    
    //Randomly generate the amount max claim of a process for each resource
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
int usrSendRequest(Request* reqArray, pid_t pid, int resIndex, int amount) {
    Request* iterator = getProcessRequestIterator(reqArray, pid);

    return 0;
}

void updateClaimMatrix(Request* reqArray) {
    Request* iterator = reqArray;

    if(reqArray == NULL) {
        fprintf(stderr, "ERROR: updateClaimMatrix : reqArray NULL\n");
        return;
    }

    int i, j;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        if(iterator->reqState != NULL_PROCESS) {
            for(j = 0; j < MAX_RESOURCES; ++j) {
                claimMat[i][j] = iterator->maxClaims[j];
            }
        }
        iterator++;
    }
}

//Function reads current allocs from the Resource Descriptor array and
//updates the matrix accordingly.
void updateAllocMatrix(ResourceDescriptor* resArray) {
    ResourceDescriptor* iterator = resArray;
    int i, j;

    //Reset alloc matrix
    initMatrix(allocMat);

    //Iterate through every resource
    for(i = 0; i < MAX_RESOURCES; ++i) {
        //Iterate through each alloc block in the currentAllocs array
        for(j = 0; j < iterator->maxAllocs; ++j) {
            //If a block is allocated...
            if(iterator->currentAllocs[j] > 0) {
                //...store it in the alloc matrix
                allocMat [getIndexOfPid(iterator->currentAllocs[j])] [i] += 1;
            }
        }
        iterator++;
    }
}

void updateAvailableVector(ResourceDescriptor* resArray) {
    ResourceDescriptor* iterator = resArray;
    
    //(max - current) allocs for each resource
    int i, j;
    int available, numAllocd;
    for(i = 0; i < MAX_RESOURCES; ++i) {
        available = 0;
        numAllocd = 0;

        //Count number of non-zero elements
        for(j = 0; j < iterator->maxAllocs; ++j) {
            if(iterator->currentAllocs[j] != 0) {
                ++numAllocd;
            }
        }
        
        //Calculate still available then put into availVec
        available = iterator->maxAllocs - numAllocd;
        availVec[i] = available;
        
        iterator++;
    }
}

void ossProcessRequests(Request* reqArray, ResourceDescriptor* resArray) {
    Request* iterator = reqArray;
    int i, j;
    int count = 0;

    //Process unproccessed requests
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {

        //Process unprocessed requests
        if(iterator->reqState == UNPROCESSED) {
            //Make state matrix a copy of allocd matrix
            copyMatrix(allocMat, stateMat);

            //Stick it in the state matrix
            stateMat[i][iterator->resource] += iterator->amount;

            //Check for safety
            if(isSafeState(iterator) == 1) {
                fprintf(stderr, "approved\n");
                approveRequest(iterator, resArray, iterator->pid);
                iterator->reqState = APPROVED;
            }
            else {
                fprintf(stderr, "denied\n");
                denyRequest(iterator, iterator->pid);
                iterator->reqState = DENIED;
            }
            ++count;
        }
        else if(iterator->reqState == FINISHED) {
            //Deallocate all resources held by PID pointed to by iterator TODO
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
        fprintf(fp, "P%.2d  ", i);
        for(j = 0; j < MAX_RESOURCES; ++j) {
            fprintf(fp, "%.2d     ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }
}

void printVector(FILE* fp, int vec[], int size) {
    int i;
    fprintf(fp, "VECTOR: ");
    for(i = 0; i < size; ++i) {
        fprintf(fp, "%.2d ", vec[i]);
    }
    fprintf(fp, "\n");
}

void copyMatrix(int src[MAX_CHILD_PROCESSES][MAX_RESOURCES], int dest[MAX_CHILD_PROCESSES][MAX_RESOURCES]){
    int i, j;
    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        for(j = 0; j < MAX_RESOURCES; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}