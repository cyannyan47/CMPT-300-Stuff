#ifndef SCR_OUT_H
#define SCR_OUT_H

#include <pthread.h>
#include "list.h"

#define MAX_LENGTH 1024

#define SCR_OUT_FAIL -1
#define SCR_OUT_SUCCESS 0

// void Scr_out_init(List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond);
void Scr_out_init();
void Scr_out_Shutdown();

void Scr_out_SignalToGetMsg();

#endif