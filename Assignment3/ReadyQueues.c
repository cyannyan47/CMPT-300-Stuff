#include "ReadyQueues.h"
#include "stdlib.h"

static List* HighPrioListP;
static List* NormPrioListP;
static List* LowPrioListP;

// pComparisonArg is an int pointer
static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == ((int*)pComparisonArg));
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

PCB* ScheduleNextPCB() {
    // Try to empty the high prio queue first
    // Then try to empty the norm prio queue
    // Then try to empty the low prio queue

    // If all three queues are empty
    // Return NULL

    // Notice: This can starve the low procs from CPU time
    // Too bad
    PCB* retPtr;
    if(List_count(HighPrioListP) != 0) {
        retPtr = List_trim(HighPrioListP);
        return retPtr;
    }

    if(List_count(HighPrioListP) != 0) {
        retPtr = List_trim(NormPrioListP);
        return retPtr;
    }

    if(List_count(HighPrioListP) != 0) {
        retPtr = List_trim(LowPrioListP);
        return retPtr;
    }

    return NULL;
}

PCB* FindInPrioHigh(int pid) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(HighPrioListP, &compare_pid, &targetPid);
    return targetPCBPtr;
}

PCB* FindInPrioNorm(int pid) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(NormPrioListP, &compare_pid, &targetPid);
    return targetPCBPtr;
}

PCB* FindInPrioLow(int pid) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(LowPrioListP, &compare_pid, &targetPid);
    return targetPCBPtr;
}

PCB* FindAndRemovePrioHigh(int pid) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(HighPrioListP, &compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        return NULL;
    }
    
    // Remove
    targetPCBPtr = List_remove(HighPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        return NULL;
    }

    return targetPCBPtr;
}
PCB* FindAndRemovePrioNorm(int pid) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(NormPrioListP, &compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        return NULL;
    }
    
    // Remove
    targetPCBPtr = List_remove(HighPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        return NULL;
    }

    return targetPCBPtr;
}
PCB* FindAndRemovePrioLow(int pid) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(LowPrioListP, &compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't find process with pid %d!\n", pid);
        return NULL;
    }
    
    // Remove
    targetPCBPtr = List_remove(HighPrioListP);
    if (targetPCBPtr == NULL) {
        printf("ReadyQueue: Can't remove process with pid %d!\n", pid);
        return NULL;
    }

    return targetPCBPtr;
}