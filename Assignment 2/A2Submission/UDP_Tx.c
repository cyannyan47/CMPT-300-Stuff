#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#include "UDP_Tx.h"
#include "List_Manager.h"
#include "Shutdown_Manager.h"

static pthread_t UDP_Tx_PID;

static int s_sockfd;
static struct addrinfo s_senderHints;
static struct addrinfo *s_Ptr;
// Variables from main
static char *s_remoteHostname, *s_remotePort;
static char *s_pMsgAllocated = NULL;

void* start_sender() {

    memset(&s_senderHints, 0, sizeof s_senderHints);
	s_senderHints.ai_family = AF_INET;
	s_senderHints.ai_socktype = SOCK_DGRAM;

    int rv;
    struct addrinfo *s_senderInfo;
    if ((rv = getaddrinfo(s_remoteHostname, s_remotePort, &s_senderHints, &s_senderInfo)) != 0) {
		fprintf(stderr, "UDP_Tx: getaddrinfo: %s\n", gai_strerror(rv));
		// return UDP_TX_FAIL;
	}

    // loop through all the results and make a socket
    for(s_Ptr = s_senderInfo; s_Ptr != NULL; s_Ptr = s_Ptr->ai_next) {
		if ((s_sockfd = socket(s_Ptr->ai_family, s_Ptr->ai_socktype,
				s_Ptr->ai_protocol)) == -1) {
			perror("UDP_Tx: socket FAILED\n");
			continue;
		}
		break;
	}

    if (s_Ptr == NULL) {
		fprintf(stderr, "UDP_Tx: FAILED to create socket\n");
	}

    freeaddrinfo(s_senderInfo);

    while(1) {
        // Trimming message from transmitter list
        Transmitter_List_trim(&s_pMsgAllocated);

        // Trimmed message is only NULL when it get signal shutdown from UDP_Rx
        if (!s_pMsgAllocated) {
            break;
        }

        // Send the message to remote
        int numbytes;
        if ((numbytes = sendto(s_sockfd, s_pMsgAllocated, strlen(s_pMsgAllocated), 0,
                s_Ptr->ai_addr, s_Ptr->ai_addrlen)) == -1) {
            perror("UDP_Tx: sendto FAILED\n");
        }
        
        // If msg = "!\n", then trigger shutdown and break the loop
        if (strcmp(s_pMsgAllocated, "!\n") == 0) {
            SM_trigger_shutdown_local(); // Trigger shut down for the other three local threads
            break;
        }
        
        free(s_pMsgAllocated);
        s_pMsgAllocated = NULL;
    }
    UDP_Tx_Shutdown();
    return NULL;
}


void UDP_Tx_init(char *remoteHostname, char *remotePort) {
    s_remoteHostname = remoteHostname;
    s_remotePort = remotePort;
    SM_load_UDP_Tx_PID(&UDP_Tx_PID);
    pthread_create(&UDP_Tx_PID, NULL, start_sender, NULL);
}

void UDP_Tx_Shutdown() {
    free(s_pMsgAllocated);
    s_pMsgAllocated = NULL;
    close(s_sockfd);
}