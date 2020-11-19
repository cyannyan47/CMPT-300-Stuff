

#ifndef SENDWAITINGQUEUE_H
#define SENDWAITINGQUEUE_H

#include "list.h"
#include "PCB.h"
#include <stdbool.h>

#define SEND_SUCCESS 0
#define SEND_FAIL -1

void Init_Send();

int AddToSendList(PCB* procP);

int FindSendInQueue(int pid, PCB** retPtr);

int FindAndRemoveSendInQueue(int pid, PCB** retPtr);

bool IsSendListEmpty();

void PrintAllSendStatus();

void CleanAllSendList();

#endif