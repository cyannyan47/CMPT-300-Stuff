#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "Kb_in.h"
#include "UDP_Tx.h"
#include "List_Manager.h"
#include "Shutdown_Manager.h"

static pthread_t kb_in_PID;

static char *s_pMsgAllocated = NULL;

void* start_kb_in() {
    int sz;
    int null_term_idx;
    
    while (1) {
        // Remember to free the malloc pointer either
		// 1: After send out the message to remote
		// 2: After a shutdown trigger
		s_pMsgAllocated = (char *)malloc(MAX_LENGTH);

        // Read from keyboard
        printf("Kb_in: Reading from keyboard.\n");
        if ((sz = read(STDOUT_FILENO, s_pMsgAllocated, MAX_LENGTH)) < 0) {
            // return error
            printf("Couldn't read from keyboard\n");
        }

        null_term_idx = sz < MAX_LENGTH ? sz : MAX_LENGTH - 1;
		s_pMsgAllocated[null_term_idx] = '\0';

        // Add to list
        Transmitter_List_prepend(s_pMsgAllocated);

        // if (strcmp(msg, "!\n") == 0) {
        //     printf("Kb_in: msg == !\\n\n");
        //     // shutdown // break;
        //     break;
        // } else {
        //     printf("Kb_in: Message is not \'!\'\n");
        // }
    }
    return NULL;
}
void Kb_in_init() {
    SM_load_Kb_in_PID(&kb_in_PID);
    pthread_create(&kb_in_PID, NULL, start_kb_in, NULL);
}

void Kb_in_Shutdown() {
    printf("Shutdown Kb_in\n");

    free(s_pMsgAllocated);
    s_pMsgAllocated = NULL;

    pthread_cancel(kb_in_PID);
    // pthread_join(kb_in_PID, NULL);
    
    // int retcode;
    // pthread_join(kb_in_PID, (void**)&retcode);

    // if (retcode == KB_IN_SUCCESS) {
    //     printf("Process terminated by remote sending \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}