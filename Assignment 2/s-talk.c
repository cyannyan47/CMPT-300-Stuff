#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "list.h"
#include "Kb_in.h"
#include "Scr_out.h"
#include "UDP_Tx.h"
#include "UDP_Rx.h"

int main (int argc, char *argv[])
{
    pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
    List* transmitter_list = List_create();
    List* receiver_list = List_create();
    pthread_cond_t UDP_Tx_cond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t UDP_Rx_cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t UDP_Tx_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t UDP_Rx_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    printf ("This program was called with \"%s\".\n", argv[0]);

    if (argc != 4) {
        printf("Usage:\ns-talk [local port number] [remote hostname] [remote port number]\n");
        return EXIT_FAILURE;
    }
    
    printf("My Port number: %s\n", argv[1]);
    printf("Want to connect to machine: %s\n", argv[2]);
    printf("With port: %s\n", argv[3]);

    // SETUP
    // Transmitter
    Kb_in_init(transmitter_list, &list_mutex, &UDP_Tx_mutex, &UDP_Tx_cond);
    UDP_Tx_init(argv[2], argv[3], transmitter_list, &list_mutex, &UDP_Tx_mutex, &UDP_Tx_cond);

    // Receiver
    UDP_Rx_init(argv[1], receiver_list, &list_mutex, &UDP_Rx_mutex, &UDP_Rx_cond);
    Scr_out_init(receiver_list, &list_mutex, &UDP_Rx_mutex, &UDP_Rx_cond);

    // CLOSE
    Kb_in_WaitForShutdown();
    Scr_out_WaitForShutdown();
    UDP_Tx_waitForShutdown();
    UDP_Rx_waitForShutdown();

    return 0;
}
