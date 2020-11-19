// PCB.h
// This file will hold all PCB struct info

#ifndef PCB_H
#define PCB_H

#include <stdbool.h>

#define MAX_MSG_LENGTH 40
#define PCB_SUCCESS 0
#define PCB_FAIL -1

typedef enum PCB_State State;
enum PCB_State {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct Comms_info_s Comms_info;
struct Comms_info_s {
    int source;
    char* msg;
};

typedef struct PCB_s PCB;
struct PCB_s {
    int pid;
    int prio;   // Only 0: high, 1: norm, 2: low
    State currState;

    Comms_info* recv_info;
    Comms_info* repl_info;
};

PCB* PCB_Create_Init();

PCB* PCB_Create(int prio);

PCB* PCB_Copy(PCB* ptrPCB);

// Run the next PCB
// Print out that the process is running
void PCB_Run(PCB* ptrPCB);

// Run recv message when PCB is running
int PCB_RunRecv(PCB* ptrPCB);

// Print out that the PCB is in ready queue
void PCB_Ready(PCB* ptrPCB);

// Print out that the PCB is being blocked
void PCB_Blocked(PCB* ptrPCB);

// Return true if currState of ptrPCB is BLOCKED
// Return false otherwise
bool PCB_IsBlocked(PCB* ptrPCB);

// Call this func when ptrPCB is successfully receiving a msg
// This ptrPCB is supposed to be the receiver
// Print out that the message is 
void PCB_RecvMsgFrom(PCB* ptrPCB, int sourcePID, char* msg);

// Call this func when ptrPCB is successfully replying to a pid
// This ptrPCB is supposed to be the sender
int PCB_ReplyToSender(PCB* ptrPCB, int sourcePID, char* msg);

void PCB_Free(PCB *ptrPCB);

void PCB_PrintInfo(PCB* ptrPCB);



#endif