#include "RecvWaitingQueue.h"
#include <stdlib.h>
#include <stdio.h>

static List* recvList;

static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == (*(int*)pComparisonArg));
}

static void listPCBFree(void *pItem)
{
    PCB_Free((PCB*)pItem);
}


void Init_Recv() {
    recvList = List_create();
}

int AddToRecvList(PCB* procP) {
    int status;
    if ((status = List_prepend(recvList, procP)) == LIST_FAIL) {
        return RECV_FAIL;
    } else {
        return RECV_SUCCESS;
    }
}

int FindRecvInQueue(int pid, PCB** retPtr) {
    int targetPid = pid;
    List_first(recvList);
    PCB* targetPCBPtr = List_search(recvList, compare_pid, &targetPid);
    if (targetPCBPtr == NULL) {
        *retPtr = NULL;
        return RECV_FAIL;
    }

    *retPtr = targetPCBPtr;
    return RECV_SUCCESS;
}

int FindAndRemoveRecvInQueue(int pid, PCB** retPtr) {
    // Find
    int targetPid = pid;
    List_first(recvList);
    PCB* targetPCBPtr = List_search(recvList, compare_pid, &targetPid);

    if (targetPCBPtr == NULL) {
        printf("RecvQueue: Can't find process with pid %d!\n", pid);
        *retPtr = NULL;
        return RECV_FAIL;
    }
    
    // Remove
    targetPCBPtr = List_remove(recvList);
    if (targetPCBPtr == NULL) {
        printf("RecvQueue: Can't remove process with pid %d!\n", pid);
        *retPtr = NULL;
        return RECV_FAIL;
    }

    *retPtr = targetPCBPtr;
    return RECV_SUCCESS;
}

bool IsRecvListEmpty() {
    if (List_count(recvList) != 0) {
        return false;
    }
    return true;
}

void PrintAllRecvStatus() {
    PCB* currProc;
    printf("Receiver processes waiting for send: <List start>\t");
    currProc = List_first(recvList);
    while (currProc != NULL) {
        printf("(%d) ->\t", currProc->pid);
        currProc = List_next(recvList);
    }
    printf("<List end>\n");
}

void CleanAllRecvList() {
    List_free(recvList, listPCBFree);
    recvList = NULL;
}