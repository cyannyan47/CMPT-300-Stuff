#ifndef UDP_RX_H_
#define UDP_RX_H_

#include "list.h"

#define _POSIX_C_SOURCE 200112L

#define UDP_RX_FAIL -1
#define UDP_RX_SUCCESS 0

#define MAX_LENGTH 1024

// void UDP_Rx_init(char* local_port, List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond);
void UDP_Rx_init(char* local_port);

void UDP_Rx_Shutdown();


#endif