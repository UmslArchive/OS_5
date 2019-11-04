//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "processManage.h"

static int pid = 0;
static int exitStatus = 0;
static pid_t* activeProccesses = NULL;

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

int areActiveProcesses() {
    int i;
    if(activeProccesses != NULL) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(activeProccesses[i] > 0)
                return 1;
        }
    }

    return 0;
}

static int activeProcessArrayFull(){
    int i;
    if(activeProccesses != NULL) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(activeProccesses[i] == 0)
                return 0;
        }
        return 1;
    }

    return -1;
}

void printActiveProcessArray() {
    int i;
    if(activeProccesses != NULL) {
        fprintf(stderr, "------------------------------------------ACTIVE-PROCESSES-------------------------------------------------\n");
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            fprintf(stderr, "%.2d    ",i);
        }
        fprintf(stderr, "\n");
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            fprintf(stderr, "%.5d ", activeProccesses[i]);
        }
        fprintf(stderr, "\n-----------------------------------------------------------------------------------------------------------\n");
        fprintf(stderr, "\n");
    }
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
    //Don't spawn process if array is full
    if(activeProcessArrayFull() == 1) {
        fprintf(stderr, "\t\t\tProcess array full--No fork\n");
        return 0;
    }

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
    return 0;
}

void waitNoBlock() {
    while((pid = waitpid(-1, &exitStatus, WNOHANG))) {
        if((pid == -1) && (errno != EINTR))
            break;
        else {
            //fprintf(stderr, "PID %d exited w/ status %d\n", pid, WEXITSTATUS(exitStatus));
            removeFromActiveProccesses(pid);
            printActiveProcessArray();
        }
    }
}

void killChildren() {
    int i;
    if(activeProccesses != NULL) {
        for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
            if(activeProccesses[i] > 0) {
                kill(activeProccesses[i], SIGTERM);
            }
        }
    }
}


//Child functions:
