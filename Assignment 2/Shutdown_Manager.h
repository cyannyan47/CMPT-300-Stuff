#ifndef SHUTDOWN_MANAGER_H
#define SHUTDOWN_MANAGER_H

#include <pthread.h>

// SM = ShutdownManager

void SM_load_Kb_in_PID(pthread_t *kb_in_PID);
void SM_load_Scr_out_PID(pthread_t *scr_out_PID);
void SM_load_UDP_Tx_PID(pthread_t *UDP_Tx_PID);
void SM_load_UDP_Rx_PID(pthread_t *UDP_Rx_PID);

void SM_wait_for_shutdown();

void SM_trigger_shutdown_local();
void SM_trigger_shutdown_remote();

#endif