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

#include "UDP_Rx.h"
#include "Scr_out.h"
#include "List_Manager.h"
#include "Shutdown_Manager.h"

static pthread_t UDP_Rx_PID;
static char* s_local_port;

static char *s_pMsgAllocated = NULL;
static int s_sockfd;

void* start_receiver() {
	
	struct addrinfo hints;
	struct addrinfo *returned_info, *curr_info;
	int rv;
	int numbytes;
	socklen_t addr_len;
	int null_term_idx;
	struct sockaddr_storage remote_addr;

    memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use local IP

	if ((rv = getaddrinfo(NULL, s_local_port, &hints, &returned_info)) != 0) {
		fprintf(stderr, "UDP_Rx: getaddrinfo: %s\n", gai_strerror(rv));
	}

    // loop through all the results and bind to the first we can
	for(curr_info = returned_info; curr_info != NULL; curr_info = curr_info->ai_next) {
		if ((s_sockfd = socket(curr_info->ai_family, curr_info->ai_socktype,
				curr_info->ai_protocol)) == -1) {
			perror("UDP_Rx: socket FAILED\n");
			continue;
		}

		if (bind(s_sockfd, curr_info->ai_addr, curr_info->ai_addrlen) == -1) {
			close(s_sockfd);
			perror("UDP_Rx: bind FAILED\n");
			continue;
		}

		break;
	}

	if (curr_info == NULL) {
		fprintf(stderr, "UDP_Rx: FAILED to bind socket\n");
	}

	freeaddrinfo(returned_info);

	// Listening from remote
	while(1) {
		// Remember to free the malloc pointer either
		// 1: After print out the message on screen
		// 2: After a shutdown trigger
		s_pMsgAllocated = (char *)malloc(MAX_LENGTH);
		addr_len = sizeof remote_addr;
		if ((numbytes = recvfrom(s_sockfd, s_pMsgAllocated, MAX_LENGTH, 0,
			(struct sockaddr *)&remote_addr, &addr_len)) == -1) {
			perror("UDP_Rx: recvfrom FAILED\n");
		}

		// Padding '\0' at the end of the message to let C recognize it as a string
		null_term_idx = numbytes < MAX_LENGTH ? numbytes : MAX_LENGTH - 1;
		s_pMsgAllocated[null_term_idx] = '\0';

		// If received message is "!\n" then signal shutdown for the other 3 threads and itself
		if (strcmp(s_pMsgAllocated, "!\n") == 0) {
			SM_trigger_shutdown_remote();
        }

		// Prepend message to receiver list
		Receiver_List_prepend(s_pMsgAllocated);
	}
	return NULL;
}

void UDP_Rx_init(char* local_port) {
	s_local_port = local_port;
	SM_load_UDP_Rx_PID(&UDP_Rx_PID);
    pthread_create(&UDP_Rx_PID, NULL, start_receiver, local_port);
}


void UDP_Rx_Shutdown() {
	free(s_pMsgAllocated);
	s_pMsgAllocated = NULL;
	close(s_sockfd);
	pthread_cancel(UDP_Rx_PID);
}