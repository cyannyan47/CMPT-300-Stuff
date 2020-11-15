#ifndef UDP_TX_H_
#define UDP_TX_H_

#include "list.h"

#define _POSIX_C_SOURCE 200112L

#define UDP_TX_FAIL -1
#define UDP_TX_SUCCESS 0

#define MAX_LENGTH 1024

// Initialize socket to send messages to remote machine
void UDP_Tx_init(char *remoteHostname, char *remotePort);

void UDP_Tx_Shutdown();


// TODO: possible functions if when have concurrency

// void UDP_Tx_init(List *pList);

// implement concurrency so that this function doesn't take char* parameter
// And just get info from the List
// void UDP_Tx_sendMessage();

// void UDP_Tx_waitForShutdown();

#endif