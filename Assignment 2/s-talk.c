#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "list.h"
#include "List_Manager.h"
#include "Kb_in.h"
#include "Scr_out.h"
#include "UDP_Tx.h"
#include "UDP_Rx.h"
#include "Shutdown_Manager.h"

int main (int argc, char *argv[])
{
    
    
    printf ("This program was called with \"%s\".\n", argv[0]);

    if (argc != 4) {
        printf("Usage:\ns-talk [local port number] [remote hostname] [remote port number]\n");
        return EXIT_FAILURE;
    }
    
    printf("My Port number: %s\n", argv[1]);
    printf("Want to connect to machine: %s\n", argv[2]);
    printf("With port: %s\n", argv[3]);

    // SETUP
    // Init List
    List_Manager_init();

    // Transmitter side
    Kb_in_init();
    UDP_Tx_init(argv[2], argv[3]);

    // Receiver side
    UDP_Rx_init(argv[1]);
    Scr_out_init();

    // CLOSE
    SM_wait_for_shutdown();

    return 0;
}
