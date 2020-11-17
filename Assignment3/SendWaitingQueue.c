#include "SendWaitingQueue.h"
#include "stdlib.h"

static List* sendList;

static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == ((int*)pComparisonArg));
} 

void Init_Send() {
    sendList = List_create();
}

int AddToSendList(PCB* procP) {
    int status;
    if ((status = List_prepend(sendList, procP)) == LIST_FAIL) {
        return SEND_FAIL;
    } else {
        return SEND_SUCCESS;
    }
}

PCB* FindSendInQueue(int pid) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, &compare_pid, &targetPid);
    return targetPCBPtr;
}

PCB* FindAndRemoveSendInQueue(int pid) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, &compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("SendQueue: Can't find process with pid %d!\n", pid);
        return NULL;
    }
    
    // Remove
    targetPCBPtr = List_remove(sendList);
    if (targetPCBPtr == NULL) {
        printf("SendQueue: Can't remove process with pid %d!\n", pid);
        return NULL;
    }

    return targetPCBPtr;
}