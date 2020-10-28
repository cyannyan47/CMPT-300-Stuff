#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "Scr_out.h"
#include "List_Manager.h"
#include "Shutdown_Manager.h"

#define MAX_LENGTH 1024

static pthread_t scr_out_PID;
static char *s_pMsgAllocated;

void* start_scr_out() {
    int sz;
    
    while(1) {
        // Wait for UDP_Rx
        // Get msg from list
        Receiver_List_trim(&s_pMsgAllocated);

        // Write to screen
        printf("Scr_out: Writing to screen\n");
        if ((sz = write(STDOUT_FILENO, s_pMsgAllocated, MAX_LENGTH)) < 0) {
            // return error
            printf("Cannot print received message.\n");
        }

        // Check if msg = "!\n" then shutdown
        // if (strcmp(msg, "!\n") == 0) {
        //     printf("Scr_out: msg == !\\n\n");
        //     // shutdown // break;
        //     break;
        // } else {
        //     printf("Scr_out: Message is not \'!\'\n");
        // }

        // Free dynamically allocated memory in UDP_Rx
        free(s_pMsgAllocated);
    }
    return NULL;
}

void Scr_out_init() {
    SM_load_Scr_out_PID(&scr_out_PID);
    pthread_create(&scr_out_PID, NULL, start_scr_out, NULL);
}

void Scr_out_Shutdown() {
    printf("Shutdown Scr_out\n");

    free(s_pMsgAllocated);
    s_pMsgAllocated = NULL;

    pthread_cancel(scr_out_PID);
    pthread_join(scr_out_PID, NULL);
    // int retcode;
    // pthread_join(scr_out_PID, (void**)&retcode);
    // // print scr_out result
    // if (retcode == SCR_OUT_SUCCESS) {
    //     printf("Process terminated by remote sending \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}

