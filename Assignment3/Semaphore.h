// Semaphore.h
// This file manages semaphores and process controlled by said semaphore

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "list.h"
#include "PCB.h"
#define MAX_SEM_AMOUNT 5

#define SEM_SUCCESS 0
#define SEM_FAIL -1

typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    int value;
    List* processList;
};

Semaphore *semaphoreList;

void Semaphore_Init();

int Semaphore_New(int semID);

int Semaphore_P(int semID, PCB* procP);

int Semaphore_V(int semID);

int Semaphore_CleanAll();

#endif