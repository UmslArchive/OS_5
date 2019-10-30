//Author:   Colby Ackerman
//Class:    Operating Systems - CS4760
//Assign:   #5
//Date:     11/5/19
//=========================================================

#include "shared.h"

//========================================================
//---------------------MAIN-------------------------------
//========================================================

int main(int arg, char* argv[]) {

    Clock c;

    initClock(&c);

    printf("%d %d\n", c.seconds, c.nanoseconds);

    tickClock(&c, 0, 2000000001);

    printf("oss hello\n");
}