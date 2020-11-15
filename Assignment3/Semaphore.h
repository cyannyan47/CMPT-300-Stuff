// Semaphore.h
// This file manages semaphores and process controlled by said semaphore

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "list.h"

typedef struct Semaphore_s Semphore;
struct Semaphore_s {
    int value;
    List processList;
};

#endif