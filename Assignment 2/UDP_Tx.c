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

#define _POSIX_C_SOURCE 200112L


#define MAX_LENGTH 1024

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
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		// return UDP_TX_FAIL;
	}

    // loop through all the results and make a socket
    for(s_Ptr = s_senderInfo; s_Ptr != NULL; s_Ptr = s_Ptr->ai_next) {
		if ((s_sockfd = socket(s_Ptr->ai_family, s_Ptr->ai_socktype,
				s_Ptr->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}

    if (s_Ptr == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		// return UDP_TX_FAIL;
	}

    freeaddrinfo(s_senderInfo);

    while(1) {
        // Wait for Kb_in
        // Get msg from list
        Transmitter_List_trim(&s_pMsgAllocated);

        // Send the msg to remote
        int numbytes;
        printf("UDP_Tx: Sending the msg to remote\n");
        if (s_pMsgAllocated == NULL) {
            printf("This s_pMsgAllocated is NULL\n");
        } else {
            printf("Not NULL %s\n", s_pMsgAllocated);
        }
        if ((numbytes = sendto(s_sockfd, s_pMsgAllocated, strlen(s_pMsgAllocated), 0,
                s_Ptr->ai_addr, s_Ptr->ai_addrlen)) == -1) {
            perror("talker: sendto");
            // return UDP_TX_FAIL;
        }
        printf("Before strcmp\n");
        // If msg = "!\n", then break the loop and return success
        if (strcmp(s_pMsgAllocated, "!\n") == 0) {
            printf("UDP_Tx: msg == !\\n\n");
            // shutdown
            SM_trigger_shutdown_local();
        }
        printf("After strcmp\n");
        free(s_pMsgAllocated);
        s_pMsgAllocated = NULL;
    }
    return NULL;
}


void UDP_Tx_init(char *remoteHostname, char *remotePort) {
    s_remoteHostname = remoteHostname;
    s_remotePort = remotePort;
    SM_load_UDP_Tx_PID(&UDP_Tx_PID);
    pthread_create(&UDP_Tx_PID, NULL, start_sender, NULL);
}

void UDP_Tx_Shutdown() {
    printf("Shutdown UDP_Tx\n");
    
    free(s_pMsgAllocated);
    s_pMsgAllocated = NULL;
    close(s_sockfd);

    pthread_cancel(UDP_Tx_PID);
    // pthread_join(UDP_Tx_PID, NULL);

    // int retcode;
    // pthread_join(UDP_Tx_PID, (void**)&retcode);
    // // print scr_out result
    // if (retcode == UDP_TX_SUCCESS) {
    //     printf("Process terminated by sending \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}