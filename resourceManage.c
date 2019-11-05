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


Request* getProcessRequestIterator(Request* reqArray, pid_t pid) {
    int index = getPidIndex(pid);

    if(index = -1) {
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
int makeRequest(Request* reqArray) {

}
