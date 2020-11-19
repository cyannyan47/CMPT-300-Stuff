// ReadyQueues.h
// This .h file will control all 3 ready queues for 3 priority

#ifndef READYQUEUES_H
#define READYQUEUES_H

#include "PCB.h"
#include "list.h"
#include <stdbool.h>

#define READYQ_SUCCESS 0
#define READYQ_FAIL -1

void Init_ReadyQueues();

int AddPCBtoPrioHigh(PCB* procP);
int AddPCBtoPrioNorm(PCB* procP);
int AddPCBtoPrioLow(PCB* procP);
int AddPCBtoReadyQueue(PCB* procP);
// Todo: somehow put init proc in here

// Decide the next PCB to run
// And pop that PCB out of its queue
// Return 0 when success, -1 when fail to find next PCB
// Make sure to run the init process when this fail
int ScheduleNextPCB(PCB** retPtr);

// Find and return elements in Queues
// For Procinfo

int FindInPrioHigh(int pid, PCB** retPtr);
int FindInPrioNorm(int pid, PCB** retPtr);
int FindInPrioLow(int pid, PCB** retPtr);
int FindInPrioAll(int pid, PCB** retPtr);

// Find and remove elements from Queues
// First step for Kill command
// Will Kill in CLIManager
// Return PCB ptr if removed, NULL if not

int FindAndRemovePrioHigh(int pid, PCB** retPtr);
int FindAndRemovePrioNorm(int pid, PCB** retPtr);
int FindAndRemovePrioLow(int pid, PCB** retPtr);

bool IsAllReadyQueueEmpty();

void PrintAllReadyQStatus();

void CleanAllReadyQueue();
#endif
