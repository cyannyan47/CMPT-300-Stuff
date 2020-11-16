#include "CLIManager.h"
#include "ReadyQueues.h"
#include "PCB.h"

static PCB* runningProc;

void CLI_Init() {
    // Init ready queue
    // Init SendWaitingQueue
    // Init RecvWaitingQueue
    // Init Semaphore

    // Start "init" process
    PCB* initProcPtr = PCB_Create_Init();
    runningProc = initProcPtr;
}

void CLI_Create(int prio) {

}

void CLI_Fork();

void CLI_Kill(int pid);

void CLI_Exit();

void CLI_Quantum();

void CLI_Send(int pid, char *msg);

void CLI_Receive();

void CLI_Reply(int pid, char *msg);

void CLI_SemNew(int semID);

void CLI_SemP(int semID);

void CLI_SemV(int semID);

void CLI_ProcInfo(int pid);

void CLI_TotalInfo();