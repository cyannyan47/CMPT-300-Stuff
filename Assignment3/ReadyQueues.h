// ReadyQueues.h
// This .h file will control all 3 ready queues for 3 priority

#ifndef READYQUEUES_H
#define READYQUEUES_H

#include "PCB.h"

void Init_ReadyQueues();

void AddPCBtoPrioHigh();

void AddPCBtoPrioNorm();

void AddPCBtoPrioLow();


#endif
