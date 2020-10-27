#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "Kb_in.h"

static pthread_t kb_in_PID;
static List *s_transmitter_list;
static pthread_mutex_t *s_list_mutex;
static pthread_mutex_t *s_UDP_Tx_mutex;
static pthread_cond_t *s_UDP_Tx_cond;

void* start_kb_in() {
    int sz;
    char msg[MAX_LENGTH];
    
    while (1) {
        // Read from keyboard
        if ((sz = read(STDOUT_FILENO, msg, MAX_LENGTH)) < 0) {
            // return error
            printf("Couldn't read from keyboard\n");
        }

        // Add to list
        pthread_mutex_lock(s_list_mutex);
		{
			if (List_prepend(s_transmitter_list, (void *)msg) == LIST_FAIL) {
                // return error
                printf("Critical Section: Prepend to List failed\n");
            }
		}
		pthread_mutex_unlock(s_list_mutex);

        // Signal UDP_Tx to get new item
        pthread_mutex_lock(s_UDP_Tx_mutex);
		{
			pthread_cond_signal(s_UDP_Tx_cond);
		}
		pthread_mutex_unlock(s_UDP_Tx_mutex);
    }
}
void Kb_in_init(List *transmitter_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Tx_mutex, pthread_cond_t *UDP_Tx_cond) {
    s_transmitter_list = transmitter_list;
    s_list_mutex = list_mutex;
    s_UDP_Tx_mutex = UDP_Tx_mutex;
    s_UDP_Tx_cond = UDP_Tx_cond;

    pthread_create(&kb_in_PID, NULL, start_kb_in, NULL);
}

void Kb_in_WaitForShutdown() {
    pthread_join(kb_in_PID, NULL);
    // int retcode;
    // pthread_join(kb_in_PID, (void**)&retcode);

    // if (retcode == KB_IN_SUCCESS) {
    //     printf("Process terminated by remote sending \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}