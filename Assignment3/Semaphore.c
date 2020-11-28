#include "Semaphore.h"
#include <stdlib.h>
#include <stdio.h>

static Semaphore* semaphoreArrayPtr[MAX_SEM_AMOUNT];

// pComparisonArg is an int pointer
static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == (*(int*)pComparisonArg));
} 

static void listPCBFree(void *pItem)
{
    PCB_Free((PCB*)pItem);
}


void Semaphore_Init() {
    for(int i = 0; i < MAX_SEM_AMOUNT; i++) { 
        semaphoreArrayPtr[i] = NULL;
    }
}

int Semaphore_New(int semID, int initValue) {
    // Check if the semID is from 0 to 4
    if (semID < 0 || semID > MAX_SEM_AMOUNT) {
        printf("Semaphore ID %d out of bound!\n", semID);
        return SEM_FAIL;
    }

    if (initValue < 0) {
        printf("Semaphore ID %d init value is negative\n", semID);
        return SEM_FAIL;
    }

    // Check if semID has already been created
    if (semaphoreArrayPtr[semID] != NULL) {
        printf("Semaphore ID %d has already been initialized!\n", semID);
        return SEM_FAIL;
    }

    // Init a new semaphore
    semaphoreArrayPtr[semID] = (Semaphore*)malloc(sizeof(Semaphore));
    semaphoreArrayPtr[semID]->value = initValue;    // Only let 1 process run per semaphore
    semaphoreArrayPtr[semID]->processList = List_create();

    return SEM_SUCCESS;
}

int Semaphore_P(int semID, PCB* procP) {
    // Check if the semID is from 0 to 4
    if (semID < 0 || semID > MAX_SEM_AMOUNT) {
        printf("Semaphore ID %d out of bound!\n", semID);
        return SEM_FAIL;
    }

    // Check if semID hasn't already been created
    if (semaphoreArrayPtr[semID] == NULL) {
        printf("Semaphore ID %d is not initialized!\n", semID);
        return SEM_FAIL;
    }

    semaphoreArrayPtr[semID]->value--;
    if (semaphoreArrayPtr[semID]->value < 0) {
        if (List_prepend(semaphoreArrayPtr[semID]->processList, procP) == LIST_FAIL) {
            printf("Semaphore ID %d add proc to list fail\n", semID);
            return SEM_FAIL;
        }
        // Change status to Blocked
        PCB_Blocked(procP);
    }

    return SEM_SUCCESS;
}

int Semaphore_V(int semID, PCB** retPtr) {
    // Check if the semID is from 0 to 4
    if (semID < 0 || semID > MAX_SEM_AMOUNT) {
        printf("Semaphore ID %d out of bound!\n", semID);
        *retPtr = NULL;
        return SEM_FAIL;
    }

    // Check if semID hasn't already been created
    if (semaphoreArrayPtr[semID] == NULL) {
        printf("Semaphore ID %d is not initialized!\n", semID);
        *retPtr = NULL;
        return SEM_FAIL;
    }

    semaphoreArrayPtr[semID]->value++;
    if(semaphoreArrayPtr[semID]->value <= 0) {
        PCB* trimProcPtr = List_trim(semaphoreArrayPtr[semID]->processList);
        *retPtr = trimProcPtr;  // Return pointer to pointer of trimmed PCB
        return SEM_SUCCESS;
    }

    *retPtr = NULL;
    return SEM_SUCCESS;
}

int Semaphore_Findpid(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = NULL;
    for (int i = 0; i < MAX_SEM_AMOUNT; i++) {
        if (semaphoreArrayPtr[i] == NULL) {
            continue;
        }
        List_first(semaphoreArrayPtr[i]->processList);
        targetPCBPtr = List_search(semaphoreArrayPtr[i]->processList, compare_pid, &targetPid);
        if (targetPCBPtr != NULL) {
            // Found the pid
            break;
        }
    }
    if (targetPCBPtr == NULL) {
        *retPtr = NULL;
        return SEM_FAIL;
    }
    *retPtr = targetPCBPtr;
    return SEM_SUCCESS;
}

int Semaphore_FindAndRemovepid(int pid, PCB** retPtr) {
    int targetPid = pid;
    PCB* targetPCBPtr = NULL;
    for (int i = 0; i < MAX_SEM_AMOUNT; i++) {
        if (semaphoreArrayPtr[i] == NULL) {
            continue;
        }
        List_first(semaphoreArrayPtr[i]->processList);
        targetPCBPtr = List_search(semaphoreArrayPtr[i]->processList, compare_pid, &targetPid);
        if (targetPCBPtr != NULL) {
            targetPCBPtr = List_remove(semaphoreArrayPtr[i]->processList);
            break;
        }
    }

    if (targetPCBPtr == NULL) {
        *retPtr = NULL;
        return SEM_FAIL;
    }
    *retPtr = targetPCBPtr;
    return SEM_SUCCESS;
}

void Semaphore_CleanAll() {
    for (int i = 0; i < MAX_SEM_AMOUNT; i++) {
        if (semaphoreArrayPtr[i] == NULL) {
            continue;
        }

        List_free(semaphoreArrayPtr[i]->processList, listPCBFree);

        free(semaphoreArrayPtr[i]);
    }
}

bool Semaphore_IsAllEmpty() {
    for (int i = 0; i < MAX_SEM_AMOUNT; i++) {
        if (semaphoreArrayPtr[i] == NULL) {
            continue;
        }

        // Found a PCB blocked inside a semaphore
        // Return false
        if (List_count(semaphoreArrayPtr[i]->processList) != 0) {
            return false;
        }
    }

    // Can't find any blocked PCB in semaphore
    // return true
    return true;
}

void Semaphore_PrintStatus() {
    PCB* currProc;
    for (int i = 0; i < MAX_SEM_AMOUNT; i++) {
        if (semaphoreArrayPtr[i] == NULL) {
            printf("Semaphore %d has not yet been initialized\n", i);
            continue;
        }
        printf("Semaphore %d, value = %d: ", i, semaphoreArrayPtr[i]->value);
        printf("<Semaphore In>\t");
        currProc = List_first(semaphoreArrayPtr[i]->processList);
        while (currProc != NULL) {
            printf("(%d) ->\t", currProc->pid);
            currProc = List_next(semaphoreArrayPtr[i]->processList);
        }
        printf("<Semaphore Out>\n");
    }
}