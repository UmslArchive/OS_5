//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"
#include "processManage.h"
#include "interrupts.h"

#define MAX_CHILDREN 18

int main(int arg, char* argv[]) {

    //Array which stores PIDs of currently active processes
    pid_t* activeProccesses = malloc(MAX_CHILDREN * sizeof(pid_t));

    //Register signal handlers
    ossInitSignalHandler();
    sigaction(SIGINT, &ossSigAction, 0);
    sigaction(SIGALRM, &ossSigAction, 0);

    //Init semaphore
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);

    pid = fork();
    if(pid == 0) {
        execl("./usr", "usr",  (char*) NULL);
    }

    while(1) {
        //Check if a signal was received
        sem_wait(sem);
        sleep(1);
        printf("oss hello\n");
        sem_post(sem);

        if(ossSignalReceivedFlag == 1)
            break;
    }

    //Cleanup
    sem_close(sem);
    free(activeProccesses);

}
