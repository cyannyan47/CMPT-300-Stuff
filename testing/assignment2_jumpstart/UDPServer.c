
// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// For getaddrinfo()
#include <netdb.h>

#define PORT 8080
#define MAXLINE 1024
#define MYPORT "6969"	// the port users will be connecting to

// Driver code
// int main() {
//     int sockfd;
//     char buffer[MAXLINE];
//     char *hello = "Hello from server";
//     struct sockaddr_in servaddr, cliaddr;

//     // Creating socket file descriptor
//     if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
//         perror("socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     memset(&servaddr, 0, sizeof(servaddr));
//     memset(&cliaddr, 0, sizeof(cliaddr));

//     // Filling server information
//     servaddr.sin_family = AF_INET; // IPv4
//     servaddr.sin_addr.s_addr = INADDR_ANY;
//     servaddr.sin_port = htons(PORT);

//     // Bind the socket with the server address
//     if ( bind(sockfd, (const struct sockaddr *)&servaddr,
//             sizeof(servaddr)) < 0 )
//     {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     int len, n;

//     len = sizeof(cliaddr);  //len is value/resuslt

//     n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//                 MSG_WAITALL, ( struct sockaddr *) &cliaddr,
//                 &len);
//     buffer[n] = '\0';
//     printf("Client : %s\n", buffer);
//     sendto(sockfd, (const char *)hello, strlen(hello),
//         MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//             len);
//     printf("Hello message sent.\n");

//     // The code above are from: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
//     // Custom code:

//     int rsz, wsz;
//     char message[MAXLINE];

//     while (1) {
//         n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//             MSG_WAITALL, ( struct sockaddr *) &cliaddr,
//             &len);
//         buffer[n] = '\0';
//         wsz = write(STDOUT_FILENO, buffer, n);
//         rsz = read(STDIN_FILENO, message, MAXLINE);
//         message[rsz] = '\0';
//         sendto(sockfd, (const char *)message, strlen(message),
//             MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
//             len);
//     }

//     return 0;
// }

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];   //buffer for receive from client
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP
    hints.ai_flags = AI_PASSIVE;    //For binding to the IP of the host the code's running on

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

    // Creating socket file descriptor
    // loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

    if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}
    // Finish using getaddrinfo
    freeaddrinfo(servinfo);

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    len = sizeof(cliaddr);  //len is value/resuslt
    printf("Server: Before first recvfrom\n");
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n");

    // The code above are from: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    // Custom code:

    int rsz, wsz;
    char message[MAXLINE];

    while (1) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
            MSG_WAITALL, ( struct sockaddr *) &cliaddr,
            &len);
        buffer[n] = '\0';
        wsz = write(STDOUT_FILENO, buffer, n);
        rsz = read(STDIN_FILENO, message, MAXLINE);
        message[rsz] = '\0';
        sendto(sockfd, (const char *)message, strlen(message),
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    }

    return 0;
}
