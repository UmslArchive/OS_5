//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"

int pid = 0;
int exitStatus = 0;
pid_t* activeProccesses = NULL;

//Static functions:
static void addToActiveProcesses() {
    int i;
    if(activeProccesses != NULL && pid != 0) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(activeProccesses[i] == 0) {
                activeProccesses[i] = pid;
                fprintf(stderr, "PID %d added to activePs array at %d\n", pid, i);
                pid = 0;
                return;
            }
        }
        fprintf(stderr, "Active process array full\n");
    }

    fprintf(stderr, "Active process array is NULL--Process not added\n");
    return;
}

static void removeFromActiveProccesses(pid_t process) {
    int i;
    if(activeProccesses != NULL) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(activeProccesses[i] == process) {
                activeProccesses[i] = 0;
                fprintf(stderr, "PID %d removed from active proccesses at %d\n", process, i);
                return;
            }
        }
        fprintf(stderr, "PID %d not found in active proccesses\n", process);
        return;
    }

    fprintf(stderr, "Active process array is NULL--Process not removed\n");
    return;
}

static int areActiveProccesses() {
    int i;
    if(activeProccesses != NULL) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        if(activeProccesses[i] > 0)
            return 1;
        }
    }

    return 0;
}

//Initialization/deallocation:
int initOssProcessManager() {
    int i;
    if(activeProccesses == NULL) {
        activeProccesses = malloc(MAX_CHILD_PROCESSES * sizeof(pid_t));
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i)
            activeProccesses[i] = 0;
        return 0;
    }
    fprintf(stderr, "initOssProcessManager() failed\n");
    return -1;
}

int destroyProcessManager() {
    if(activeProccesses != NULL) {
        free(activeProccesses);
        activeProccesses = NULL;
        return 0;
    }
    fprintf(stderr, "destroyProcessManager() failed\n");
    return -1;
}


//oss functions:

int spawnProcess() {
    pid = fork();

    //Handle fork error
    if(pid < 0) {
        perror("ERROR:oss:failed to fork");
        return -1;
    }

    //Exec child
    if(pid == 0) {
        execl("./usr", "usr",  (char*) NULL);
    }

    addToActiveProcesses();
}

int waitWhileStillActiveProcesses() {
    while(areActiveProccesses()) {
        pid = wait(&exitStatus);

        fprintf(stderr, "PID %d exited w/ status %d\n", pid, WEXITSTATUS(exitStatus));
        removeFromActiveProccesses(pid);
        pid = 0;
    }
}


//Child functions:
