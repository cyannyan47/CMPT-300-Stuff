#include "UDP_Rx.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

static int s_sockfd;
static struct addrinfo s_receiverHints;
static struct addrinfo *s_receiverInfo, *s_Ptr;
static struct sockaddr_in s_remoteAddr;

// Variables from main
static char* s_receivePort;

int start_listener(char* receivePort) {
    memset(&s_receiverHints, 0, sizeof s_receiverHints);
	s_receiverHints.ai_family = AF_INET; // set to AF_INET to force IPv4
	s_receiverHints.ai_socktype = SOCK_DGRAM;
	s_receiverHints.ai_flags = AI_PASSIVE; // use my IP

    int rv;
	if ((rv = getaddrinfo(NULL, receivePort, &s_receiverHints, &s_receiverInfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return UDP_RX_FAIL;
	}

    // loop through all the results and bind to the first we can
	for(s_Ptr = s_receiverInfo; s_Ptr != NULL; s_Ptr = s_Ptr->ai_next) {
		if ((s_sockfd = socket(s_Ptr->ai_family, s_Ptr->ai_socktype,
				s_Ptr->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(s_sockfd, s_Ptr->ai_addr, s_Ptr->ai_addrlen) == -1) {
			close(s_sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (s_Ptr == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return UDP_RX_FAIL;
	}

	freeaddrinfo(s_receiverInfo);

    return UDP_RX_SUCCESS;
}

void UDP_Rx_init(char* receivePort) {
    s_receivePort = receivePort;
    pthread_t threadPID;
    pthread_create(&threadPID, NULL, start_listener, receivePort);
}

char* UDP_Rx_receiveMessage() {

}

void UDP_Rx_waitForShutdown() {

}