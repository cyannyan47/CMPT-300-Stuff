#include "PCB.h"
#include <stdlib.h>

static int pidCounter;

PCB* PCB_Create_Init() {
    pidCounter = 0;
    PCB* retPtr = (PCB*)malloc(sizeof(PCB));

    // Assigning "init" PCB values
    retPtr->currState = RUNNING;
    retPtr->pid = pidCounter;
    retPtr->prio = 0;
    retPtr->proc_msg = NULL;

    // Update unique pidCounter
    pidCounter++;

    return retPtr;
}

PCB* PCB_Create(int prio) {
    PCB* retPtr = (PCB*)malloc(sizeof(PCB));

    // Assigning new PCB values
    retPtr->currState = READY;
    retPtr->pid = pidCounter;
    retPtr->prio = prio;
    retPtr->proc_msg = NULL;

    // Update unique pidCounter
    pidCounter++;
    
    return retPtr;
}

PCB* PCB_Copy(PCB* ptrPCB) {
    PCB* retPtr = (PCB*)malloc(sizeof(PCB));

    retPtr->currState = READY;
    retPtr->pid = pidCounter;
    retPtr->prio = ptrPCB->prio;
    retPtr->proc_msg = ptrPCB->proc_msg;

    // Update unique pidCounter
    pidCounter++;

    return retPtr;
}

void PCB_Free(PCB *ptrPCB) {
    if (ptrPCB->proc_msg != NULL) {
        free(ptrPCB->proc_msg);
        ptrPCB->proc_msg = NULL;
    }

    free(ptrPCB);
}