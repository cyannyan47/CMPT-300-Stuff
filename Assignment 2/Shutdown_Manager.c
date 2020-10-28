#include <pthread.h>

#include "Shutdown_Manager.h"
#include "List_Manager.h"
#include "Kb_in.h"
#include "Scr_out.h"
#include "UDP_Rx.h"
#include "UDP_Tx.h"

// SM = ShutdownManager

// Local PID for shutdown management
static pthread_t *s_kb_in_PID;
static pthread_t *s_scr_out_PID;
static pthread_t *s_UDP_Tx_PID;
static pthread_t *s_UDP_Rx_PID;

void SM_load_Kb_in_PID(pthread_t *kb_in_PID) {
    s_kb_in_PID = kb_in_PID;
}

void SM_load_Scr_out_PID(pthread_t *scr_out_PID) {
    s_scr_out_PID = scr_out_PID;
}

void SM_load_UDP_Tx_PID(pthread_t *UDP_Tx_PID) {
    s_UDP_Tx_PID = UDP_Tx_PID;
}

void SM_load_UDP_Rx_PID(pthread_t *UDP_Rx_PID) {
    s_UDP_Rx_PID = UDP_Rx_PID;
}

void SM_wait_for_shutdown() {
    pthread_join(*s_kb_in_PID, NULL);
    pthread_join(*s_scr_out_PID, NULL);
    pthread_join(*s_UDP_Tx_PID, NULL);
    pthread_join(*s_UDP_Rx_PID, NULL);
}

void SM_trigger_shutdown_local() {
    Kb_in_Shutdown();
    UDP_Rx_Shutdown();
    Scr_out_Shutdown();
    UDP_Tx_Shutdown();

    List_Manager_shutdown();
}

void SM_trigger_shutdown_remote() {
    Kb_in_Shutdown();
    UDP_Tx_Shutdown();
    Scr_out_Shutdown();
    UDP_Rx_Shutdown();

    List_Manager_shutdown();
}