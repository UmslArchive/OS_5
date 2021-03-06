//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#ifndef SHARED_H
#define SHARED_H

#include "processManage.h"

//Keys
extern const key_t SHM_KEY_SEM;     
extern const key_t SHM_KEY_CLOCK;
extern const key_t SHM_KEY_RESOURCE;
extern const key_t SHM_KEY_REQUEST;
extern const key_t SHM_KEY_MSG;
extern const key_t SHM_KEY_RRA;

//IDs
extern int shmSemID;
extern int shmClockID;
extern int shmResourceDescID;
extern int shmRequestID;
extern int shmMsgID;
extern int shmRRAID;

//Sizes
extern const size_t shmSemSize;
extern const size_t shmClockSize;
extern const size_t shmResourceDescSize;
extern const size_t shmRequestSize;
extern const size_t shmMsgSize;
extern const size_t shmRRASize;

//Oss Flags
extern const int SHM_OSS_FLAGS;
extern const int SHM_USR_FLAGS;

//----------------------------------------

//Structs
typedef struct clock_struct {
    unsigned int seconds;
    unsigned int nanoseconds;
} Clock;

typedef enum msg_state { 
    SENT,
    RECEIVED,
    EMPTY
} MsgState;

typedef struct msg_struct {
    MsgState state;
    pid_t usrpid;
    int index;
} Msg;

typedef struct req_req_struct {
    pid_t requesterPid;
    MsgState state;
    Clock timestamp;
} ReqReq;

typedef struct req_req_arr_struct {
    ReqReq requestRequests[MAX_CHILD_PROCESSES];
} RRA;

//Shared mem init functions
sem_t* initShmSemaphore(const key_t key, const size_t size, int* shmid, int flags);
void* initSharedMemory(const key_t key, const size_t size, int* shmid, int flags);

//Cleanup functions
void detachAll();
void cleanupSharedMemory(int* shmid);
void cleanupAll();

//Clock functions
void initClock(Clock* clock);
void setClock(Clock* clock, unsigned int sec, unsigned int nanosec);
void advanceClock(Clock* mainClock, unsigned int sec, unsigned int nanosec);
Clock timeDifference(Clock* subtractFrom, Clock* subtractAmount);
void printClock(Clock* clock);
int checkIfPassedTime(Clock* mainClock, Clock* timeLimit);

//Msg functions
void resetMsg(Msg* msg);

//Wack
void initRRA(RRA* rra);
void usrSendRequestRequest(RRA* rra, pid_t pid, int apIndex, Clock* mainClock);

//Decided to do all Resource/Request functions 
//inside of the resourceManage.* files

#endif