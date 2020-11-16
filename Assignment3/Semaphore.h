// Semaphore.h
// This file manages semaphores and process controlled by said semaphore

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "list.h"

typedef struct Semaphore_s Semaphore;
struct Semaphore_s {
    int value;
    List processList;
};

Semaphore *semaphoreList;

void Semaphore_init();

void Semaphore_P(int semID);

void Semaphore_V(int semID);

#endif