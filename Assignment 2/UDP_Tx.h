#ifndef _UDP_TX_H_
#define _UDP_TX_H_

#define UDP_TX_FAIL -1
#define UDP_TX_SUCCESS 0

#include "list.h"

// Initialize socket to send messages to remote machine
void UDP_Tx_init(char *remoteHostname, char *remotePort, List *trasmitter_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Tx_mutex, pthread_cond_t *UDP_Tx_cond);

void UDP_Tx_waitForShutdown();


// TODO: possible functions if when have concurrency

// void UDP_Tx_init(List *pList);

// implement concurrency so that this function doesn't take char* parameter
// And just get info from the List
// void UDP_Tx_sendMessage();

// void UDP_Tx_waitForShutdown();

#endif