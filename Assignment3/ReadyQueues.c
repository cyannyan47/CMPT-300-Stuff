#include "ReadyQueues.h"
#include <stdlib.h>

static List* HighPrioListP;
static List* NormPrioListP;
static List* LowPrioListP;

// pComparisonArg is an int pointer
static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == ((int*)pComparisonArg));
}

static void listPCBFree(void *pItem)
{
    PCB_Free((PCB*)pItem);
}


void Init_ReadyQueues() {
    // Create 3 ready queues
    HighPrioListP = List_create();
    NormPrioListP = List_create();
    LowPrioListP = List_create();
}

int AddPCBtoPrioHigh(PCB* procP) {
    int status;
    if ((status = List_prepend(HighPrioListP, procP)) == LIST_FAIL) {
        return READYQ_FAIL;
    } else {
        return READYQ_SUCCESS;
    }
}

int AddPCBtoPrioNorm(PCB* procP) {
    int status;
    if ((status = List_prepend(NormPrioListP, procP)) == LIST_FAIL) {
        return READYQ_FAIL;
    } else {
        return READYQ_SUCCESS;
    }
}

int AddPCBtoPrioLow(PCB* procP) {
    int status;
    if ((status = List_prepend(LowPrioListP, procP)) == LIST_FAIL) {
        return READYQ_FAIL;
    } else {
        return READYQ_SUCCESS;
    }
}

int AddPCBtoReadyQueue(PCB* procP) {
    int status;
    int result;
    if (procP->currState != READY) {
        PCB_Ready(procP);
    }
    switch (procP->prio) {
        case 0:
            if ((status = List_prepend(HighPrioListP, procP)) == LIST_FAIL) {
                result = READYQ_FAIL;
            } else {
                result = READYQ_SUCCESS;
            }
            break;
        case 1:
            if ((status = List_prepend(NormPrioListP, procP)) == LIST_FAIL) {
                result = READYQ_FAIL;
            } else {
                result = READYQ_SUCCESS;
            }
            break;
        case 2:
            if ((status = List_prepend(LowPrioListP, procP)) == LIST_FAIL) {
                result = READYQ_FAIL;
            } else {
                result = READYQ_SUCCESS;
            }
            break;
        default:
            printf("Invalid priority value %d!\n", procP->prio);
            result = READYQ_FAIL;
            break;
    }
    return result;
}

int ScheduleNextPCB(PCB** retPtr) {
    // Try to empty the high prio queue first
    // Then try to empty the norm prio queue
    // Then try to empty the low prio queue

    // If all three queues are empty
    // Return NULL

    // Notice: This can starve the low procs from CPU time
    // Too bad
    PCB* trimPCBPtr;
    if(List_count(HighPrioListP) != 0) {
        trimPCBPtr = List_trim(HighPrioListP);
        *retPtr = trimPCBPtr;
        return READYQ_SUCCESS;
    }

    if(List_count(HighPrioListP) != 0) {
        trimPCBPtr = List_trim(NormPrioListP);
        *retPtr = trimPCBPtr;
        return READYQ_SUCCESS;
    }

    if(List_count(HighPrioListP) != 0) {
        trimPCBPtr = List_trim(LowPrioListP);
        *retPtr = trimPCBPtr;
        return READYQ_SUCCESS;
    }

    *retPtr = NULL;
    return READYQ_FAIL;
}

int FindInPrioHigh(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(HighPrioListP, compare_pid, &targetPid);
    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

int FindInPrioNorm(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(NormPrioListP, compare_pid, &targetPid);
    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

int FindInPrioLow(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(LowPrioListP, compare_pid, &targetPid);
    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

int FindInPrioAll(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(HighPrioListP, compare_pid, &targetPid);
    if (targetPCBPtr != NULL) {
        *retPtr = targetPCBPtr;
        return READYQ_SUCCESS;
    }
    PCB* targetPCBPtr = List_search(NormPrioListP, compare_pid, &targetPid);
    if (targetPCBPtr != NULL) {
        *retPtr = targetPCBPtr;
        return READYQ_SUCCESS;
    }
    PCB* targetPCBPtr = List_search(LowPrioListP, compare_pid, &targetPid);
    if (targetPCBPtr != NULL) {
        *retPtr = targetPCBPtr;
        return READYQ_SUCCESS;
    }

    *retPtr = NULL;
    return READYQ_FAIL;
}

int FindAndRemovePrioHigh(int pid, PCB** retPtr) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(HighPrioListP, compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }
    
    // Remove
    targetPCBPtr = List_remove(HighPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }

    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

int FindAndRemovePrioNorm(int pid, PCB** retPtr) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(NormPrioListP, compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }
    
    // Remove
    targetPCBPtr = List_remove(NormPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }

    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

int FindAndRemovePrioLow(int pid, PCB** retPtr) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(LowPrioListP, compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }
    
    // Remove
    targetPCBPtr = List_remove(LowPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        *retPtr = NULL;
        return READYQ_FAIL;
    }

    *retPtr = targetPCBPtr;
    return READYQ_SUCCESS;
}

bool IsAllReadyQueueEmpty() {
    if (List_count(HighPrioListP) != 0) {
        return false;
    }
    if (List_count(NormPrioListP) != 0) {
        return false;
    }
    if (List_count(LowPrioListP) != 0) {
        return false;
    }

    return true;
}

void PrintAllReadyQStatus() {
    PCB* currProc;
    printf("High priority queue: <Queue in>\t");
    currProc = List_first(HighPrioListP);
    while (currProc != NULL) {
        printf("(%d) ->\t", currProc->pid);
        List_next(HighPrioListP);
    }
    printf("<Queue Out>\n");

    printf("Normal priority queue: <Queue in>\t");
    currProc = List_first(NormPrioListP);
    while (currProc != NULL) {
        printf("(%d) ->\t", currProc->pid);
        List_next(NormPrioListP);
    }
    printf("<Queue Out>\n");

    printf("Low priority queue: <Queue in>\t");
    currProc = List_first(LowPrioListP);
    while (currProc != NULL) {
        printf("(%d) ->\t", currProc->pid);
        List_next(LowPrioListP);
    }
    printf("<Queue Out>\n");
}

void CleanAllReadyQueue() {
    List_free(HighPrioListP, listPCBFree);
    HighPrioListP = NULL;
    List_free(NormPrioListP, listPCBFree);
    NormPrioListP = NULL;
    List_free(LowPrioListP, listPCBFree);
    LowPrioListP = NULL;
}