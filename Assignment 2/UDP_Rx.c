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

#define _POSIX_C_SOURCE 200112L


#define MAX_LENGTH 1024

static pthread_t UDP_Rx_PID;
static List *s_receiver_list;
static pthread_mutex_t *s_list_mutex;
static pthread_mutex_t *s_UDP_Rx_mutex;
static pthread_cond_t *s_UDP_Rx_cond;
static char *s_remote_hostname;
static char* s_remote_port;

void* start_receiver() {
	int sockfd;
	struct addrinfo hints;
	struct addrinfo *returned_info, *curr_info;
	int rv;
	int numbytes;
	socklen_t addr_len;
	char msg[MAX_LENGTH];
	int null_term_idx;
	struct sockaddr_storage remote_addr;

    memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use local IP

	if ((rv = getaddrinfo(NULL, s_remote_port, &hints, &returned_info)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		//return UDP_RX_FAIL;
	}

    // loop through all the results and bind to the first we can
	for(curr_info = returned_info; curr_info != NULL; curr_info = curr_info->ai_next) {
		if ((sockfd = socket(curr_info->ai_family, curr_info->ai_socktype,
				curr_info->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, curr_info->ai_addr, curr_info->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (curr_info == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		//return UDP_RX_FAIL;
	}

	freeaddrinfo(returned_info);

	// Listening from remote
	while(1) {
		printf("UDP_Rx: Listening (waiting) from remote\n");
		addr_len = sizeof remote_addr;
		if ((numbytes = recvfrom(sockfd, msg, MAX_LENGTH, 0,
			(struct sockaddr *)&remote_addr, &addr_len)) == -1) {
			perror("recvfrom");
			// return UDP_RX_FAIL;
		}

		null_term_idx = numbytes < MAX_LENGTH ? numbytes : MAX_LENGTH - 1;
		msg[null_term_idx] = '\0';

		// Add msg to list
		pthread_mutex_lock(s_list_mutex);
		{
			printf("UDP_Rx: Adding msg to list\n");
			if (List_prepend(s_receiver_list, msg) == LIST_FAIL) {
				// return error
			}
		}
		pthread_mutex_unlock(s_list_mutex);

		// Signal scr_out
		pthread_mutex_lock(s_UDP_Rx_mutex);
		{
			printf("UDP_Rx: Signalling scr_out\n");
			pthread_cond_signal(s_UDP_Rx_cond);
		}
		pthread_mutex_unlock(s_UDP_Rx_mutex);
	}
	return NULL;
    // return UDP_RX_SUCCESS;
}

void UDP_Rx_init(char *remote_hostname, char* remote_port, List *receiver_list, pthread_mutex_t *list_mutex, pthread_mutex_t *UDP_Rx_mutex, pthread_cond_t *UDP_Rx_cond) {
	s_remote_hostname = remote_hostname;
	s_receiver_list = receiver_list;
    s_list_mutex = list_mutex;
	s_UDP_Rx_mutex = UDP_Rx_mutex;
    s_UDP_Rx_cond = UDP_Rx_cond;
	s_remote_port = remote_port;
    pthread_create(&UDP_Rx_PID, NULL, start_receiver, remote_port);
}


void UDP_Rx_waitForShutdown() {
	pthread_join(UDP_Rx_PID, NULL);
	// int retcode;
    // pthread_join(UDP_Rx_PID, (void**)&retcode);
    // // print scr_out result
    // if (retcode == UDP_RX_SUCCESS) {
    //     printf("Process terminated by receiving \"!\"\n");
    // } else {
    //     printf("There was an error in the process\n");
    // }
}