#include "RecvWaitingQueue.h"
#include "stdlib.h"

static List* sendList;

static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == ((int*)pComparisonArg));
} 

void Init_Recv() {
    sendList = List_create();
}

int AddToRecvList(PCB* procP) {
    int status;
    if ((status = List_prepend(sendList, procP)) == LIST_FAIL) {
        return RECV_FAIL;
    } else {
        return RECV_SUCCESS;
    }
}

PCB* FindRecvInQueue(int pid) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, &compare_pid, &targetPid);
    return targetPCBPtr;
}

PCB* FindAndRemoveRecvInQueue(int pid) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, &compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("RecvQueue: Can't find process with pid %d!\n", pid);
        return NULL;
    }
    
    // Remove
    targetPCBPtr = List_remove(sendList);
    if (targetPCBPtr == NULL) {
        printf("RecvQueue: Can't remove process with pid %d!\n", pid);
        return NULL;
    }

    return targetPCBPtr;
}