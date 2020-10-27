#ifndef SCR_OUT_H
#define SCR_OUT_H

#include <pthread.h>
#include "list.h"

#define SCR_OUT_FAIL -1
#define SCR_OUT_SUCCESS 0

void scr_out_init(List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond);
void scr_out_WaitForShutdown();

#endif