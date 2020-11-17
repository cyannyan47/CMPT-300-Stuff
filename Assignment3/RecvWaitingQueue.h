
#ifndef RECVWAITINGQUEUE_H
#define RECVWAITINGQUEUE_H

#include "list.h"
#include "PCB.h"

#define RECV_SUCCESS 0
#define RECV_FAIL -1

void Init_Recv();

int AddToRecvList(PCB* procP);

PCB* FindRecvInQueue(int pid);

PCB* FindAndRemoveRecvInQueue(int pid);


#endif