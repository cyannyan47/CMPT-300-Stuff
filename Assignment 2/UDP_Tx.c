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
#define _POSIX_C_SOURCE 200112L


#define MAX_LENGTH 1024

static pthread_t UDP_Tx_PID;

static int s_sockfd;
static struct addrinfo s_senderHints;
static struct addrinfo *s_Ptr;
static List *s_transmitter_list;
static pthread_mutex_t *s_list_mutex;
static pthread_mutex_t *s_UDP_Tx_mutex;
static pthread_cond_t *s_UDP_Tx_cond;

// Variables from main
static char *s_remoteHostname, *s_remotePort;

void* start_sender() {
    void *item;
    char msg[MAX_LENGTH];

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
        pthread_mutex_lock(s_UDP_Tx_mutex);
		{
			pthread_cond_wait(s_UDP_Tx_cond, s_UDP_Tx_mutex);
		}
		pthread_mutex_unlock(s_UDP_Tx_mutex);

        // Get msg from list
        pthread_mutex_lock(s_list_mutex);
		{
			item = List_trim(s_transmitter_list);
		}
		pthread_mutex_unlock(s_list_mutex);

        strcpy(msg, (const char *)item);

        // Send the msg to remote
        int numbytes;
        if ((numbytes = sendto(s_sockfd, msg, strlen(msg), 0,
                s_Ptr->ai_addr, s_Ptr->ai_addrlen)) == -1) {
            perror("talker: sendto");
            // return UDP_TX_FAIL;
        }

        // If msg = "!\n", then break the loop and return success
        if (strcmp(msg, "!\n") == 0) {
            // shutdown
            break;
        }
    }
    return NULL;
    // return UDP_TX_SUCCESS;
}

void UDP_Tx_init(char *remoteHostname, char *remotePort, List *transmitter_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Tx_mutex, pthread_cond_t *UDP_Tx_cond) {
    s_remoteHostname = remoteHostname;
    s_remotePort = remotePort;
    s_transmitter_list = transmitter_list;
    s_list_mutex = list_mutex;
    s_UDP_Tx_mutex = UDP_Tx_mutex;
    s_UDP_Tx_cond = UDP_Tx_cond;
    pthread_create(&UDP_Tx_PID, NULL, start_sender, NULL);
}

void UDP_Tx_waitForShutdown() {

    pthread_join(UDP_Tx_PID, NULL);
    // int retcode;
    // pthread_join(UDP_Tx_PID, (void**)&retcode);
    // // print scr_out result
    // if (retcode == UDP_TX_SUCCESS) {
    //     printf("Process terminated by sending \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}