#include "Semaphore.h"
#include "stdlib.h"

static Semaphore* semaphoreArrayPtr[MAX_SEM_AMOUNT];

// pComparisonArg is an int pointer
static bool compare_pid(void *pItem, void *pComparisonArg)
{
    return (((PCB*)pItem)->pid == ((int*)pComparisonArg));
} 


void Semaphore_Init() {
    for(int i = 0; i < MAX_SEM_AMOUNT; i++) { 
        semaphoreArrayPtr[i] = NULL;
    }
}

int Semaphore_New(int semID) {
    // Check if the semID is from 0 to 4
    if (semID < 0 || semID > MAX_SEM_AMOUNT) {
        printf("Semaphore ID %d out of bound!\n", semID);
        return SEM_FAIL;
    }

    // Check if semID has already been created
    if (semaphoreArrayPtr[semID] != NULL) {
        printf("Semaphore ID %d has already been initialized!\n", semID);
        return SEM_FAIL;
    }

    // Init a new semaphore
    semaphoreArrayPtr[semID] = (Semaphore*)malloc(sizeof(Semaphore));
    semaphoreArrayPtr[semID]->value = 1;    // Only let 1 process run per semaphore
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
            printf("Semaphore ID %d add to list fail\n", semID);
            return SEM_FAIL;
        }
    }
}

int Semaphore_V(int semID) {
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

}


