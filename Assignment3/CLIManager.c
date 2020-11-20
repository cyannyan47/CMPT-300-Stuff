#include "CLIManager.h"
#include "ReadyQueues.h"
#include "PCB.h"
#include "RecvWaitingQueue.h"
#include "SendWaitingQueue.h"
#include "Semaphore.h"

#include <stdlib.h>
#include <stdio.h>

static PCB* runningProc;
static PCB* initProc;
bool isInitExited;

void CLI_Init() {
    // Init ready queue
    Init_ReadyQueues();
    // Init SendWaitingQueue
    Init_Send();
    // Init RecvWaitingQueue
    Init_Recv();
    // Init Semaphore
    Semaphore_Init();

    // Start "init" process
    PCB* initProcPtr = PCB_Create_Init();
    runningProc = initProcPtr;
    initProc = initProcPtr;
    isInitExited = false;
}

bool CLI_IsInitExited() {
    return isInitExited;
}

void CLI_Create(int prio) {
    // Create a PCB struct
    // Newly created process is READY by default
    if (prio < 0 || prio > 2) {
        printf("Fail: Invalid prio int\n");
        return;
    }
    PCB* newProcPtr = PCB_Create(prio);

    // Run the process immediately when the process init is running
    if (runningProc == initProc) {
        PCB_Run(newProcPtr);
        runningProc = newProcPtr;

        // Make init proc ready
        PCB_Ready(initProc);
        printf("Success: Running the newly created process from init process\n");
        return;
    }

    // Put the new PCB in the ready queue
    PCB_Ready(newProcPtr);
    if (AddPCBtoReadyQueue(newProcPtr) == READYQ_FAIL) {
        printf("Fail: Fail to add the new process to ready queue\n");
        printf("Terminating process instance\n");
        PCB_Free(newProcPtr);
        newProcPtr = NULL;
    }
    printf("Success: Created a new process pid %d and added to the ready queue\n", newProcPtr->pid);
}

void CLI_Fork() {
    // Check if running proc is init proc
    if (runningProc == initProc) {
        printf("Can't fork the init process!\n");
        return;
    }
    PCB* newProcPtr = PCB_Copy(runningProc);

    // Put the new PCB in the ready queue
    PCB_Ready(newProcPtr);
    if (AddPCBtoReadyQueue(newProcPtr) == READYQ_FAIL) {
        printf("Fail: Fail to add the new process to ready queue\n");
        printf("Terminating process instance\n");
        PCB_Free(newProcPtr);
        newProcPtr = NULL;
    }
    printf("Success: Forked a new process and added to the ready queue\n");
}

void CLI_Kill(int pid) {
    if (pid == 0) {
        printf("Can't kill the init process!\n");
        return;
    }

    if (runningProc->pid == pid) {
        // Prepare the next process in ready queue
        PCB* nextPCB;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }

        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    PCB* procP;
    int status;
    // Check every ready/block queue
    // Find PCB with pid in highprio Q
    status = FindAndRemovePrioHigh(pid, &procP);
    // Find PCB with pid in normprio Q
    if (status == READYQ_FAIL) {
        status = FindAndRemovePrioNorm(pid, &procP);
    }
    // Find PCB with pid in lowprio Q
    if (status == READYQ_FAIL) {
        status = FindAndRemovePrioLow(pid, &procP);
    }
    // Find PCB with pid in Recv queue
    if (status == READYQ_FAIL) {
        status = FindAndRemoveRecvInQueue(pid, &procP);
    }
    // Find PCB with pid in Send queue
    if (status == RECV_FAIL) {
        status = FindAndRemoveSendInQueue(pid, &procP);
    }
    // Find PCB with pid in Semaphores list
    if (status == SEND_FAIL) {
        status = Semaphore_FindAndRemovepid(pid, &procP);
    }

    if (status == SEM_FAIL) {
        printf("Fail: Can't find process with pid %d in system. Cancelling Kill command\n", pid);
        return;
    } else {
        printf("Found process pid %d\nFreeing process\n", pid);

        PCB_Free(procP);
        procP = NULL;

        printf("Success: Killed process with pid %d in system.\n", pid);
        return;
    }
}

void CLI_Exit() {
    // If running process is init
    if (runningProc == initProc) {
        // Check if there is only init process left in the simulation
        if (Semaphore_IsAllEmpty() && IsAllReadyQueueEmpty() && IsSendListEmpty() && IsRecvListEmpty()) {
            isInitExited = true;
            PCB_Free(runningProc);
            initProc = NULL;
            runningProc = NULL;
            return;
        }
        
    } else {
        // If running proc is not init proc
        // Get the next proc in ready queue
        PCB* nextProcPtr;
        if (ScheduleNextPCB(&nextProcPtr) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        PCB_Run(nextProcPtr);
        runningProc = nextProcPtr;
    }
}

void CLI_Quantum() {
    PCB* nextPCB;
    if (runningProc == initProc) {
        printf("Init proc is running.\n");
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nContinue to run init\n");
            return;
        }
        
        printf("Found next process in ready queue\nRunning the new process\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    printf("Scheduling the next non-init process\n");
    if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
        printf("Can't find next proc in ready queue\nRunning init process\n");
        PCB_Run(initProc);
        runningProc = initProc;
        return;
    }

    printf("Putting the current running process back to the ready queue\n");
    PCB* nextInQueue = runningProc;
    PCB_Ready(nextInQueue);
    if (AddPCBtoReadyQueue(nextInQueue) == READYQ_FAIL) {
        printf("Fail: Fail to add the new process to ready queue\n");
        printf("Continue to run the next process\n");
    }
    
    printf("Running the next process in ready queue\n");
    PCB_Run(nextPCB);
    runningProc = nextPCB;
    return;
}

void CLI_Send(int pid, char *msg) {
    if (pid == 0) {
        printf("Can't send to init process\n");
        return;
    }

    PCB* receiverProcPtr = NULL;
    // From running proc sendTo another proc
    // Can't send a message to itself
    if (runningProc->pid == pid) {
        // Record the message in receiving PCB
        PCB_RecvMsgFrom(receiverProcPtr, runningProc->pid, msg);

        // BLocking the running process since it has to wait for a reply from someone
        PCB_Blocked(runningProc);
        if (AddToSendList(runningProc) == SEND_FAIL) {
            printf("Fail to add running process to send waiting list\n");
        };

        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    // Check if the receiving process is waiting for a message
    // If the receiving process is waiting
    int stopWaitRecv = FindAndRemoveRecvInQueue(pid, &receiverProcPtr);
    if (stopWaitRecv == RECV_SUCCESS) {
        // Found and unblock the receiving process from waiting
        // Put the receiving block on the ready queue      
        PCB_RecvMsgFrom(receiverProcPtr, runningProc->pid, msg);
        PCB_Ready(receiverProcPtr);
        AddPCBtoReadyQueue(receiverProcPtr);
        
        // BLocking the running process since it has to wait for a reply from someone
        PCB_Blocked(runningProc);
        if (AddToSendList(runningProc) == SEND_FAIL) {
            printf("Fail to add running process to send waiting list\n");
        };

        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    // If the receiving process is not waiting recv
    // Check if the process is in send
    int foundWaitSend = FindSendInQueue(pid, &receiverProcPtr);
    if (foundWaitSend == SEND_SUCCESS) {
        // Record the message in receiving PCB
        PCB_RecvMsgFrom(receiverProcPtr, runningProc->pid, msg);

        // BLocking the running process since it has to wait for a reply from someone
        PCB_Blocked(runningProc);
        if (AddToSendList(runningProc) == SEND_FAIL) {
            printf("Fail to add running process to send waiting list\n");
        };

        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    int foundWaitSem = Semaphore_Findpid(pid, &receiverProcPtr);
    if (foundWaitSem == SEM_SUCCESS) {
        // Record the message in receiving PCB
        PCB_RecvMsgFrom(receiverProcPtr, runningProc->pid, msg);

        // BLocking the running process since it has to wait for a reply from someone
        PCB_Blocked(runningProc);
        if (AddToSendList(runningProc) == SEND_FAIL) {
            printf("Fail to add running process to send waiting list\n");
        };

        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }

    int foundReadyQ = FindInPrioAll(pid, &receiverProcPtr);
    if (foundReadyQ == SEM_SUCCESS) {
        // Record the message in receiving PCB
        PCB_RecvMsgFrom(receiverProcPtr, runningProc->pid, msg);

        // BLocking the running process since it has to wait for a reply from someone
        PCB_Blocked(runningProc);
        if (AddToSendList(runningProc) == SEND_FAIL) {
            printf("Fail to add running process to send waiting list\n");
        };

        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
        return;
    }
}

void CLI_Receive() {
    if (runningProc == initProc) {
        printf("Can't receive from init process\n");
        return;
    }

    int status = PCB_RunRecv(runningProc);
    if (status == PCB_SUCCESS) {
        return;
    }

    // In case running process having doesn't have a sender
    // Blocked to wait for a sender

    // Blocking the running process
    PCB_Blocked(runningProc);
    if (AddToRecvList(runningProc) == RECV_FAIL) {
        printf("Fail to add running process to send waiting list\n");
    };

    PCB* nextPCB;
    // Get the next process from scheduler
    runningProc = NULL;
    if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
        printf("Can't find next proc in ready queue\nRunning init process\n");
        PCB_Run(initProc);
        runningProc = initProc;
        return;
    }
    printf("Running the next process in ready queue\n");
    PCB_Run(nextPCB);
    runningProc = nextPCB;
    return; 
}

void CLI_Reply(int pid, char *msg) {
    // Init process can run this function

    // Check if sender pid is waiting
    PCB* senderProcPtr;
    int status = FindAndRemoveSendInQueue(pid, &senderProcPtr);
    if (status == SEND_FAIL) {
        printf("Can't reply to no sender\n");
        return;
    }
    PCB_Ready(senderProcPtr);
    status = PCB_ReplyToSender(senderProcPtr, pid, msg);
    if (status == PCB_FAIL) {
        return;
    }
}

void CLI_SemNew(int semID) {
    int status = Semaphore_New(semID);
    if (status == SEM_FAIL) {
        printf("N command failed\n");
    }
}

void CLI_SemP(int semID) {
    if (runningProc == initProc) {
        printf("Fail: init process can't use P\n");
        return;
    }

    int status = Semaphore_P(semID, runningProc);
    if (status == SEM_FAIL) {
        printf("P command failed\n");
        return;
    }

    // Check if runningProc is blocked yet
    if (PCB_IsBlocked(runningProc)) {
        printf("Running process is BLOCKED\n");
        printf("Scheduling next process\n");
        PCB* nextPCB;
        // Get the next process from scheduler
        runningProc = NULL;
        if (ScheduleNextPCB(&nextPCB) == READYQ_FAIL) {
            printf("Can't find next proc in ready queue\nRunning init process\n");
            PCB_Run(initProc);
            runningProc = initProc;
            return;
        }
        printf("Running the next process in ready queue\n");
        PCB_Run(nextPCB);
        runningProc = nextPCB;
    }
    
    printf("Running process is not BLOCKED\nContinue running process\n");
    return;
}

void CLI_SemV(int semID) {
    PCB* UnblockedProc;
    int status = Semaphore_V(semID, &UnblockedProc);
    if (status == SEM_FAIL) {
        printf("V command failed\n");
        return;
    }

    // If V command unblocks a process
    if (UnblockedProc != NULL) {
        printf("Unblocking process %d", UnblockedProc->pid);
        PCB_Ready(UnblockedProc);
        if (AddPCBtoReadyQueue(UnblockedProc) == READYQ_FAIL) {
            printf("Can't add the unblocked process to ready queue\n");
            printf("Force terminate the process\n");
            PCB_Free(UnblockedProc);
            UnblockedProc = NULL;
        }
    }
}

void CLI_ProcInfo(int pid) {
    PCB* targetPCB;
    int status = FindInPrioAll(pid, &targetPCB);
    if (status == READYQ_SUCCESS) {
        printf("Process %d is in the Ready queue\n", pid);
        PCB_PrintInfo(targetPCB);
    }

    status = FindRecvInQueue(pid, &targetPCB);
    if (status == RECV_SUCCESS) {
        printf("Process %d is waiting for a message from a sender\n", pid);
        PCB_PrintInfo(targetPCB);
    }

    status = FindSendInQueue(pid, &targetPCB);
    if (status == SEND_SUCCESS) {
        printf("Process %d is waiting for a reply\n", pid);
        PCB_PrintInfo(targetPCB);
    }

    status = Semaphore_Findpid(pid, &targetPCB);
    if (status == SEM_SUCCESS) {
        printf("Process %d is blocked by a semaphore\n", pid);
        PCB_PrintInfo(targetPCB);
    }
}

void CLI_TotalInfo() {
    printf("Ready queue status:\n");
    PrintAllReadyQStatus();
    printf("\nSemaphore queue status:\n");
    Semaphore_PrintStatus();
    printf("\nProcess Blocked by receive:\n");
    PrintAllRecvStatus();
    printf("\nProcess Blocked by send:\n");
    PrintAllSendStatus();
    printf("\n");
}

void CLI_End() {
    printf("Cleaning Ready Queue\n");
    CleanAllReadyQueue();
    printf("Cleaning Semaphores\n");
    Semaphore_CleanAll();
    printf("Cleaning waiting receiver processes\n");
    CleanAllRecvList();
    printf("Cleaning waiting sender processes\n");
    CleanAllSendList();
}