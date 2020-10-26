#include "UDP_Tx.h"

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

static pthread_t threadPID;

static int s_sockfd;
static struct addrinfo s_senderHints;
static struct addrinfo *s_Ptr;

// Variables from main
static char *s_remoteHostname, *s_remotePort;

void start_sender() {
    memset(&s_senderHints, 0, sizeof s_senderHints);
	s_senderHints.ai_family = AF_INET;
	s_senderHints.ai_socktype = SOCK_DGRAM;

    int rv;
    struct addrinfo *s_senderInfo;
    if ((rv = getaddrinfo(s_remoteHostname, s_remotePort, &s_senderHints, &s_senderInfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return UDP_TX_FAIL;
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
		return UDP_TX_FAIL;
	}

    freeaddrinfo(s_senderInfo);

    while(1) {
        
    }

    return UDP_TX_SUCCESS;
}

void UDP_Tx_init(char *remoteHostname, char *remotePort) {
    s_remoteHostname = remoteHostname;
    s_remotePort = remotePort;

    pthread_create(&threadPID, NULL, start_sender, NULL);
}

// void UDP_Tx_sendMessage();
int UDP_Tx_sendMessage(char *msg_to) {
    int numbytes;
    if ((numbytes = sendto(s_sockfd, msg_to, strlen(msg_to), 0,
            s_Ptr->ai_addr, s_Ptr->ai_addrlen)) == -1) {
        perror("talker: sendto");
        return UDP_TX_FAIL;
	}

    return UDP_TX_SUCCESS;
}

void UDP_Tx_waitForShutdown() {

}