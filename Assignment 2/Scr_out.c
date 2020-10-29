#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "Scr_out.h"
#include "List_Manager.h"
#include "Shutdown_Manager.h"

static pthread_t scr_out_PID;
static char *s_pMsgAllocated = NULL;

void* start_scr_out() {
    int sz;
    
    while(1) {
        // Trim message from receiver list
        Receiver_List_trim(&s_pMsgAllocated);

        // Trimmed message is only NULL when it get signal shutdown from UDP_Tx or UDP_Rx
        if (!s_pMsgAllocated) {
            break;
        }

        // Write to screen
        if ((sz = write(STDOUT_FILENO, s_pMsgAllocated, MAX_LENGTH)) < 0) {
            printf("Scr_out: Cannot print received message.\n");
        }

        // Free dynamically allocated memory in UDP_Rx
        free(s_pMsgAllocated);
        s_pMsgAllocated = NULL;
    }
    Scr_out_Shutdown();
    return NULL;
}

void Scr_out_init() {
    SM_load_Scr_out_PID(&scr_out_PID);
    pthread_create(&scr_out_PID, NULL, start_scr_out, NULL);
}

void Scr_out_Shutdown() {
    free(s_pMsgAllocated);
    s_pMsgAllocated = NULL;
}

