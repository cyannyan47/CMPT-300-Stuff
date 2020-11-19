
#ifndef RECVWAITINGQUEUE_H
#define RECVWAITINGQUEUE_H

#include "list.h"
#include "PCB.h"
#include <stdbool.h>

#define RECV_SUCCESS 0
#define RECV_FAIL -1

void Init_Recv();

// Make sure to update procP before using this function
int AddToRecvList(PCB* procP);

int FindRecvInQueue(int pid, PCB** retPtr);

int FindAndRemoveRecvInQueue(int pid, PCB** retPtr);

bool IsRecvListEmpty();

void PrintAllRecvStatus();

void CleanAllRecvList();

#endif