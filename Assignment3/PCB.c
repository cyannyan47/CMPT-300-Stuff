#include "PCB.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int pidCounter;

PCB* PCB_Create_Init() {
    pidCounter = 0;
    PCB* retPtr = (PCB*)malloc(sizeof(PCB));

    // Assigning "init" PCB values
    retPtr->currState = RUNNING;
    retPtr->pid = pidCounter;
    retPtr->prio = 0;

    retPtr->recv_info = NULL;
    retPtr->repl_info = NULL;

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

    retPtr->recv_info = NULL;
    retPtr->repl_info = NULL;

    // Update unique pidCounter
    pidCounter++;
    
    return retPtr;
}

PCB* PCB_Copy(PCB* ptrPCB) {
    PCB* retPtr = (PCB*)malloc(sizeof(PCB));

    retPtr->currState = READY;
    retPtr->pid = pidCounter;
    retPtr->prio = ptrPCB->prio;

    retPtr->recv_info = ptrPCB->recv_info;
    retPtr->repl_info = ptrPCB->repl_info;

    // Update unique pidCounter
    pidCounter++;

    return retPtr;
}

void PCB_Run(PCB* ptrPCB) {
    printf("Running process pid %d\n", ptrPCB->pid);
    ptrPCB->currState = RUNNING;
    if (ptrPCB->repl_info != NULL) {
        printf("Received reply: %s\nFrom pid: %d", ptrPCB->repl_info->msg, ptrPCB->repl_info->source);

        free(ptrPCB->repl_info);
        ptrPCB->repl_info = NULL;
    }
}

int PCB_RunRecv(PCB* ptrPCB) {
    if (ptrPCB->currState != RUNNING) {
        printf("Can't receive message because process is not running\n");
        return PCB_FAIL;
    }
    if (ptrPCB->recv_info == NULL) {
        printf("Can't receive message because process don't have any message\n");
        return PCB_FAIL;
    }

    // "Running" the receive command
    printf("Received message: %s\nFrom pid: %d", ptrPCB->recv_info->msg, ptrPCB->recv_info->source);

    // Clearing recv_info after successfully received message
    free(ptrPCB->recv_info);
    ptrPCB->recv_info = NULL;

    return PCB_SUCCESS;
}

void PCB_Ready(PCB* ptrPCB) {
    ptrPCB->currState = READY;
}

void PCB_Blocked(PCB* ptrPCB) {
    ptrPCB->currState = BLOCKED;
}

bool PCB_IsBlocked(PCB* ptrPCB) {
    if (ptrPCB->currState == BLOCKED) {
        return true;
    }
    return false;
}

void PCB_RecvMsgFrom(PCB* ptrPCB, int sourcePID, char* msg) {
    // Initialize if PCB doesn't hold any message from any sender
    if (ptrPCB->recv_info == NULL) {
        ptrPCB->recv_info = (Comms_info*)malloc(sizeof(Comms_info));
        ptrPCB->recv_info->msg = (char*)malloc(sizeof(char) * MAX_MSG_LENGTH);
    }

    ptrPCB->recv_info->source = sourcePID;
    strcpy(ptrPCB->recv_info->msg, msg);
}

int PCB_ReplyToSender(PCB* ptrPCB, int sourcePID, char* msg) {
    if (ptrPCB->currState != RUNNING) {
        printf("Can't reply message because process is not running\n");
        return PCB_FAIL;
    }
    // Initialize if PCB doesn't hold any message from any sender
    if (ptrPCB->recv_info == NULL) {
        ptrPCB->recv_info = (Comms_info*)malloc(sizeof(Comms_info));
        ptrPCB->recv_info->msg = (char*)malloc(sizeof(char) * MAX_MSG_LENGTH);
    }

    ptrPCB->recv_info->source = sourcePID;
    strcpy(ptrPCB->recv_info->msg, msg);
    return PCB_SUCCESS;
}

void PCB_Free(PCB *ptrPCB) {
    if (ptrPCB->recv_info != NULL) {
        free(ptrPCB->recv_info);
        ptrPCB->recv_info = NULL;
    }

    if (ptrPCB->repl_info != NULL) {
        free(ptrPCB->repl_info);
        ptrPCB->repl_info = NULL;
    }

    free(ptrPCB);
}

void PCB_PrintInfo(PCB* ptrPCB) {
    printf("Process ID: %d\n", ptrPCB->pid);
    char state[10];
    switch (ptrPCB->currState)
    {
    case RUNNING:
        strcpy(state, "RUNNING");
        break;
    case READY:
        strcpy(state, "READY");
        break;
    case BLOCKED:
        strcpy(state, "BLOCKED");
        break;
    default:
        break;
    }
    printf("Process current state: %s\n", state);
    printf("Process priority (0.High; 1.Norm; 2.Low): %d\n", ptrPCB->prio);
}