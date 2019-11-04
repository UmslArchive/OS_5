#include "processManage.h"
#include "interrupts.h"
#include "resourceManage.h"

void testTimeDifference() {
    Clock first;
    Clock second;
    Clock result;

    //Test case of no carry
    first.seconds = 1;
    first.nanoseconds = 5;
    second.seconds = 1;
    second.nanoseconds = 1;

    //Test label
    printf("Time Difference Tests:\n");
    
    printf("1:000000005 - 1:000000001 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    //Test case of carry
    first.seconds = 5;
    first.nanoseconds = 5;
    second.seconds = 0;
    second.nanoseconds = 10;

    printf("\n5:000000005 - 0:000000010 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    //Test underflow
    first.seconds = 1;
    first.nanoseconds = 0;
    second.seconds = 2;
    second.nanoseconds = 1;

    printf("\n1:000000000 - 2:000000001 = \n");
    result = timeDifference(&first, &second);
    printClock(&result);

    printf("-----\n");
}

void testTimeHasPassed() {
    Clock mainClock;
    Clock timeLimit;
    int result;

    //Test Label
    printf("Time has come tests:\n");

    //Test 1ns before
    mainClock.seconds = 0;
    mainClock.nanoseconds = 999999999;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 0:999999999 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test exact match
    mainClock.seconds = 1;
    mainClock.nanoseconds = 0;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 1:000000000 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test 1ns after
    mainClock.seconds = 1;
    mainClock.nanoseconds = 1;
    timeLimit.seconds = 1;
    timeLimit.nanoseconds = 0;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 1:000000001 after or at 1:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    printf("-----\n");
}

int main() {
    printf("\nRUNNING TESTS:\n-----\n");

    testTimeDifference();
    testTimeHasPassed();

    return 0;
}