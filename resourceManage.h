//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef RES_MAN_H
#define RES_MAN_H

#include "shared.h"

//Just the alloc matrix updated with newest set of requests 
typedef struct system_state_struct {

} SystemState;

typedef struct alloc_mat_struct {

} AllocMatrix;

typedef struct claim_mat_struct {

} ClaimMatrix;

typedef struct res_vec_struct {

} ResourceVector;

typedef struct avail_vec_struct {

} AvailableVector;

//Initialization
void initRequestArray();
void initResourceDescriptorArray();

//oss process resource functions:
void getRequests(Request* reqArray);
int isSafeState(SystemState* whatIfState);
void allocRequests(Request* reqArray);

//usrPs process resource functions:
int makeRequest(Request* reqArray);

//Utility:
Request* getProcessRequestIterator(Request* reqArray, pid_t pid);

#endif