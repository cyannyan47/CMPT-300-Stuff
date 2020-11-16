#include "ReadyQueues.h"
#include "stdlib.h"

static List* HighPrioListP;
static List* NormPrioListP;
static List* LowPrioListP;

void Init_ReadyQueues() {
    // Create 3 ready queues
    HighPrioListP = List_create();
    NormPrioListP = List_create();
    LowPrioListP = List_create();
}

void AddPCBtoPrioHigh(PCB* procP) {
    List_prepend(HighPrioListP, procP);
}

void AddPCBtoPrioNorm(PCB* procP) {
    List_prepend(NormPrioListP, procP);
}

void AddPCBtoPrioLow(PCB* procP) {
    List_prepend(LowPrioListP, procP);
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