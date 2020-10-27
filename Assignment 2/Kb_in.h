#ifndef KB_IN_H
#define KB_IN_H

#include <pthread.h>
#include "list.h"

#define MAX_LENGTH 1024

#define KB_IN_FAIL -1
#define KB_IN_SUCCESS 0

void Kb_in_init(List *trasmitter_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Tx_mutex, pthread_cond_t *UDP_Tx_cond);

void Kb_in_WaitForShutdown();

#endif