
// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 8080
#define MAXLINE 1024

// Driver code
// int main() {
//     int sockfd;
//     char buffer[MAXLINE];
//     char *hello = "Hello from client";
//     struct sockaddr_in servaddr;

//     // Creating socket file descriptor
//     if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
//         perror("socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     memset(&servaddr, 0, sizeof(servaddr));

//     // Filling server information
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(PORT);
//     servaddr.sin_addr.s_addr = INADDR_ANY;

//     int n, len;

//     sendto(sockfd, (const char *)hello, strlen(hello),
//         MSG_CONFIRM, (const struct sockaddr *) &servaddr,
//             sizeof(servaddr));
//     printf("Hello message sent.\n");

//     n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//                 MSG_WAITALL, (struct sockaddr *) &servaddr,
//                 &len);
//     buffer[n] = '\0';
//     printf("Server : %s\n", buffer);

//     // The code above are from: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
//     // Custom code:

//     int rsz, wsz;
//     char message[MAXLINE];

//     while (1) {
//         rsz = read(STDIN_FILENO, message, MAXLINE);
//         message[rsz] = '\0';
//         sendto(sockfd, (const char *)message, strlen(message),
//             MSG_CONFIRM, (const struct sockaddr *) &servaddr,
//             sizeof(servaddr));
//         n = recvfrom(sockfd, (char *)buffer, MAXLINE,
//             MSG_WAITALL, (struct sockaddr *) &servaddr,
//             &len);
//         buffer[n] = '\0';
//         wsz = write(STDOUT_FILENO, buffer, n);
//     }

//     close(sockfd);
//     return 0;
// }

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in servaddr;

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;  // will point to the results

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    printf("Client: Before first sendto\n");
    sendto(sockfd, (const char *)hello, strlen(hello),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            sizeof(servaddr));
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    // The code above are from: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    // Custom code:

    int rsz, wsz;
    char message[MAXLINE];

    while (1) {
        rsz = read(STDIN_FILENO, message, MAXLINE);
        message[rsz] = '\0';
        sendto(sockfd, (const char *)message, strlen(message),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
            sizeof(servaddr));
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
            MSG_WAITALL, (struct sockaddr *) &servaddr,
            &len);
        buffer[n] = '\0';
        wsz = write(STDOUT_FILENO, buffer, n);
    }

    close(sockfd);
    return 0;
}

