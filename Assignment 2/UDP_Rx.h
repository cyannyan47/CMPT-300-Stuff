#ifndef _UDP_RX_H_
#define _UDP_RX_H_

#define UDP_RX_FAIL -1
#define UDP_RX_SUCCESS 0

#include "list.h"

void UDP_Rx_init(char* remote_port, List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond);

void UDP_Rx_waitForShutdown();


#endif