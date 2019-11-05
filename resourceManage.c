//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "resourceManage.h"

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
