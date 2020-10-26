#ifndef _UDP_RX_H_
#define _UDP_RX_H_

#define UDP_RX_FAIL -1
#define UDP_RX_SUCCESS 0

#include "list.h"

void UDP_Rx_init(char* receivePort);

char* UDP_Rx_receiveMessage();

void UDP_Rx_waitForShutdown();

// void UDP_Rx_init(List *pList);

// void UDP_Rx_receiveMessage();

// void UDP_Rx_waitForShutdown();

#endif