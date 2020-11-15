// PCB.h
// This file will hold all PCB struct info

#ifndef PCB_H
#define PCB_H

#include "list.h" 

typedef enum PCB_State State;
enum PCB_State {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int prio;   // Only 0: high, 1: norm, 2: low
    State currState;
    
};

#endif