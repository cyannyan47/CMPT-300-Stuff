#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "Scr_out.h"

#define MAX_LENGTH 1024

static pthread_t scr_out_PID;
static List *s_receiver_list;
static pthread_mutex_t *s_list_mutex;
static pthread_mutex_t *s_UDP_Rx_mutex;
static pthread_cond_t *s_UDP_Rx_cond;

void* start_scr_out() {
    int sz;
    char msg[MAX_LENGTH];
    void *item;
    while(1) {
        // Wait for UDP_Rx
        pthread_mutex_lock(s_UDP_Rx_mutex);
		{
			pthread_cond_wait(s_UDP_Rx_cond, s_UDP_Rx_mutex);
		}
		pthread_mutex_unlock(s_UDP_Rx_mutex);

        // Get msg from list
        pthread_mutex_lock(s_list_mutex);
		{
			item = List_trim(s_receiver_list);
		}
		pthread_mutex_unlock(s_list_mutex);

        // Write to screen
        strcpy(msg, (const char *)item);
        if ((sz = write(STDOUT_FILENO, msg, MAX_LENGTH)) < 0) {
            // return error
            printf("Cannot print received message.\n");
        }

        // Check if msg = "!\n" then shutdown
        if (strcmp(msg, "!\n") == 0) {
            // shutdown // break;
            break;
        }
    }
    return NULL;
}

void Scr_out_init(List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond) {
    s_receiver_list = receiver_list;
    s_list_mutex = list_mutex;
    s_UDP_Rx_mutex = UDP_Rx_mutex;
    s_UDP_Rx_cond = UDP_Rx_cond;
    pthread_create(&scr_out_PID, NULL, start_scr_out, NULL);
}

void Scr_out_WaitForShutdown() {
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