//Author: Colby Ackerman

#include "resourceManage.h"

void testTimeDifference();
void testTimeHasPassed();
void testClockSet();
void testRequestArrayInit();
void testResourceDescriptorInit();
void testProcessSendRequest();
void testBuildMatrices();
void nonConcurrentProgramSimulation();

//===========================================================


int main() {
    srand(time(NULL));

    printf("\nRUNNING TESTS:\n-----\n");

    //Clock tests
    testTimeDifference();
    testTimeHasPassed();
    testClockSet();

    //Resource tests
    testRequestArrayInit();
    testResourceDescriptorInit();
    testProcessSendRequest();
    //testBuildMatrices();

    return 0;
}


//==========================================================

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

    //Test only seconds after
    mainClock.seconds = 6;
    mainClock.nanoseconds = 100;
    timeLimit.seconds = 5;
    timeLimit.nanoseconds = 100;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 6:000000000 after or at 5:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    //Test only seconds before
    mainClock.seconds = 4;
    mainClock.nanoseconds = 100;
    timeLimit.seconds = 5;
    timeLimit.nanoseconds = 100;

    result = checkIfPassedTime(&mainClock, &timeLimit);
    printf("Is 4:000000000 after or at 5:000000000?\t");
    result == 1 ? printf("YES\n") : printf("NO\n");

    printf("-----\n");
}

void testClockSet() {
    Clock clock;
    initClock(&clock);

    //Test label
    printf("Set clock tests:\n");

    //nanosec to0 high test
    printf("Attempting to set clock to 1:1000000000\n");
    setClock(&clock, 1, 1000000000);
    printClock(&clock);

    //Edge case
    printf("\nAttempting to set clock to 17:999999999\n");
    setClock(&clock, 17, 999999999);
    printClock(&clock);

    printf("-----\n");
}

void testRequestArrayInit() {
    int i;

    initOssProcessManager();
    Request* req = malloc(shmRequestSize);

    fprintf(stderr, "Request array init test:\n");

    for(i = 0; i < 25; ++i) {
        spawnDummyProcess();
    }

    initRequestArray(req);  

    //Test printing the entire array
    printAllRequests(req);

    fprintf(stderr, "\n");

    //Test a few arbitrary single prints indices = { -1, 0, 10, MAX_CHILD_PROCESSES - 1, MAX_CHILD_PROCESSES }
    printRequest(req, getPidOfIndex(-1));
    printRequest(req, getPidOfIndex(0));
    printRequest(req, getPidOfIndex(10));
    printRequest(req, getPidOfIndex(MAX_CHILD_PROCESSES - 1));
    printRequest(req, getPidOfIndex(MAX_CHILD_PROCESSES));

    free(req);
    destroyProcessManager();

    fprintf(stderr, "-----\n");
}

void testResourceDescriptorInit() {
    int i;
    ResourceDescriptor* desc = malloc(shmResourceDescSize);
    initOssProcessManager();

    initResourceDescriptorArray(desc);

    fprintf(stderr, "Resource descriptor init test:\n");

    printAllResDesc(desc);

    fprintf(stderr, "\n");

    //Test a few arbitrary single prints indices = { -1, 0, 10, MAX_RESOURCES - 1, MAX_RESOURCES }
    printResDesc(desc, -1);
    printResDesc(desc, 0);
    printResDesc(desc, 10);
    printResDesc(desc, MAX_RESOURCES - 1);
    printResDesc(desc, MAX_RESOURCES);

    destroyProcessManager();
    free(desc);

    fprintf(stderr, "-----\n");
}

void testProcessSendRequest() {
    int i;
    initMatrix(claimMat);

    initOssProcessManager();
    Request* req = malloc(shmRequestSize);
    ResourceDescriptor* desc = malloc(shmResourceDescSize);
    initResourceDescriptorArray(desc);
    initRequestArray(req);

    fprintf(stderr, "Request send tests:\n");

    printAllResDesc(desc);

    for(i = 0; i < MAX_CHILD_PROCESSES; ++i) {
        spawnDummyProcess();
        usrOnSpawnRequest(getPidOfIndex(i), req, desc);
        updateClaimMatrix(req);
    }


    printAllRequests(req);

    fprintf(stderr, "CLAIM MATRIX:\n");
    printMatrix(stderr, claimMat);

    fprintf(stderr, "\n");

    printAllRequests(req);

    updateStateMatrix(req);


    fprintf(stderr, "STATE MATRIX:\n");
    printMatrix(stderr, stateMat);


    destroyProcessManager();
    free(req);
    free(desc);

    fprintf(stderr, "-----\n");
}

void testBuildMatrices() {
    int testMatrix[MAX_CHILD_PROCESSES][MAX_RESOURCES];
    initMatrix(testMatrix);
    printMatrix(stderr, testMatrix);
}

void nonConcurrentProgramSimulation() {
    /* spawnDummyProcess();
    if(state == NULL_PROCESS)
        usrOnSpawnRequest();

    while(1)
        if(state == UNPROCESSED)
        ossRetrieveRequests();
        
        if(state == APPROVED || DENIED)
        usrSendRequest(); */


}