

#ifndef SENDWAITINGQUEUE_H
#define SENDWAITINGQUEUE_H

#include "list.h"
#include "PCB.h"

#define SEND_SUCCESS 0
#define SEND_FAIL -1

void Init_Send();

int AddToSendList(PCB* procP);

PCB* FindSendInQueue(int pid);

PCB* FindAndRemoveSendInQueue(int pid);

#endif