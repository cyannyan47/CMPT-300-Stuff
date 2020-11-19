#include "SendWaitingQueue.h"
#include <stdlib.h>
#include <stdio.h>

static List* sendList;

static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == (*(int*)pComparisonArg));
}

static void listPCBFree(void *pItem)
{
    PCB_Free((PCB*)pItem);
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

int FindSendInQueue(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, compare_pid, &targetPid);
    *retPtr = targetPCBPtr;
    return SEND_SUCCESS;
}

int FindAndRemoveSendInQueue(int pid, PCB** retPtr) {
    // Find
    int targetPid = pid;
    PCB* targetPCBPtr = List_search(sendList, compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("SendQueue: Can't find process with pid %d!\n", pid);
        *retPtr = NULL;
        return SEND_FAIL;
    }
    
    // Remove
    targetPCBPtr = List_remove(sendList);
    if (targetPCBPtr == NULL) {
        printf("SendQueue: Can't remove process with pid %d!\n", pid);
        *retPtr = NULL;
        return SEND_FAIL;
    }

    *retPtr = targetPCBPtr;
    return SEND_SUCCESS;
}

bool IsSendListEmpty() {
    if (List_count(sendList) != 0) {
        return false;
    }
    return true;
}

void PrintAllSendStatus() {
    PCB* currProc;
    printf("Sender processes waiting for reply: <List start>\t");
    currProc = List_first(sendList);
    while (currProc != NULL) {
        printf("(%d) ->\t", currProc->pid);
        List_next(sendList);
    }
    printf("<List end>\n");
}

void CleanAllSendList() {
    List_free(sendList, listPCBFree);
    sendList = NULL;
}