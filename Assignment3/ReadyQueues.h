// ReadyQueues.h
// This .h file will control all 3 ready queues for 3 priority

#ifndef READYQUEUES_H
#define READYQUEUES_H

#include "PCB.h"
#include "list.h"

void Init_ReadyQueues();

void AddPCBtoPrioHigh(PCB* procP);

void AddPCBtoPrioNorm(PCB* procP);

void AddPCBtoPrioLow(PCB* procP);

// Decide the next PCB to run
// And pop that PCB out of its queue
PCB* ScheduleNextPCB();

#endif
