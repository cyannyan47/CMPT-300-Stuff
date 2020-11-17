#include "CLIManager.h"
#include "ReadyQueues.h"
#include "PCB.h"

static PCB* runningProc;
bool isInitExited;

void CLI_Init() {
    // Init ready queue
    // Init SendWaitingQueue
    // Init RecvWaitingQueue
    // Init Semaphore

    // Start "init" process
    PCB* initProcPtr = PCB_Create_Init();
    runningProc = initProcPtr;
    isInitExited = false;
}

bool CLI_IsInitExited() {
    return isInitExited;
}

void CLI_Create(int prio) {
    // Create a PCB struct
    PCB* newProcPtr = PCB_Create(prio);

    // Put the new PCB in the ready queue
    switch(prio) {
        case 0:
            AddPCBtoPrioHigh(newProcPtr);
            break;
        case 1:
            AddPCBtoPrioNorm(newProcPtr);
            break;
        case 2:
            AddPCBtoPrioLow(newProcPtr);
            break;
        default:
            printf("Invalid priority value %d!\n", prio);
            // Demalloc newProcPtr
            PCB_Free(newProcPtr);
    }
}

void CLI_Fork() {
    if (runningProc->pid == 0) {
        printf("Can't fork the init process!\n");
        return;
    }
    PCB* newProcPtr = PCB_Copy(runningProc);

    // Put the new PCB in the ready queue
    switch(newProcPtr->prio) {
        case 0:
            AddPCBtoPrioHigh(newProcPtr);
            break;
        case 1:
            AddPCBtoPrioNorm(newProcPtr);
            break;
        case 2:
            AddPCBtoPrioLow(newProcPtr);
            break;
        default:
            printf("Invalid priority value %d while forking!\n", newProcPtr->prio);
            // Demalloc newProcPtr
            PCB_Free(newProcPtr);
    }
}

void CLI_Kill(int pid) {
    
}

void CLI_Exit();

void CLI_Quantum();

void CLI_Send(int pid, char *msg);

void CLI_Receive();

void CLI_Reply(int pid, char *msg);

void CLI_SemNew(int semID);

void CLI_SemP(int semID);

void CLI_SemV(int semID);

void CLI_ProcInfo(int pid);

void CLI_TotalInfo();