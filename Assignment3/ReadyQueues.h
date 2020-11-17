// ReadyQueues.h
// This .h file will control all 3 ready queues for 3 priority

#ifndef READYQUEUES_H
#define READYQUEUES_H

#include "PCB.h"
#include "list.h"

#define READYQ_SUCCESS 0
#define READYQ_FAIL -1

void Init_ReadyQueues();

int AddPCBtoPrioHigh(PCB* procP);
int AddPCBtoPrioNorm(PCB* procP);
int AddPCBtoPrioLow(PCB* procP);

// Decide the next PCB to run
// And pop that PCB out of its queue
PCB* ScheduleNextPCB();

// Find and return elements in Queues
// For Procinfo

PCB* FindInPrioHigh(int pid);
PCB* FindInPrioNorm(int pid);
PCB* FindInPrioLow(int pid);

// Find and remove elements from Queues
// First step for Kill command
// Will Kill in CLIManager
// Return PCB ptr if removed, NULL if not

PCB* FindAndRemovePrioHigh(int pid);
PCB* FindAndRemovePrioNorm(int pid);
PCB* FindAndRemovePrioLow(int pid);

#endif
