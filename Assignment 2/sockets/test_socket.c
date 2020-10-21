#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    int status;
    struct addrinfo hints;  // what server you want to find
    struct addrinfo *servinfo;  // will point to the results, what actually found

    memset(&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_INET;     // using IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP stream sockets
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me, used for bind()

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    } else {
        printf("getaddrinfo success\n");
        void *addr;
        char *ipver;
        char ipstr[INET6_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)servinfo->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
        inet_ntop(servinfo->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    // servinfo now points to a linked list of 1 or more struct addrinfos

    // ... do everything until you don't need servinfo anymore ....
    
    // Using socket
    int sockfd;
    // Assuming the first servinfo is the right addr we want
    // Creating an endpoint for communication from our addr 
    sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    printf("The socket file descriptor num is: %d \n", sockfd);

    // Using bind
    int bindinfo;
    // bind our socket to the port we passed in to getaddrinfo():
    bindinfo = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    printf("The bindinfo num is: %d \n", bindinfo);

    freeaddrinfo(servinfo); // free the linked-list

    printf("Before connect code\n");
    //  Using connect
    // Connect to some addr
    struct addrinfo hints2, *res;
    int sockfd2;
    // first, load up address structs with getaddrinfo():

    memset(&hints2, 0, sizeof hints2);
    hints2.ai_family = AF_INET;
    hints2.ai_socktype = SOCK_STREAM;

    printf("After setting up hints2\n");

    int addrStatus;
    if((addrStatus = getaddrinfo("www.example.com", "3490", &hints2, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    } else {
        printf("getaddrinfo with hints2 success\n");
        void *addr;
        char *ipver;
        char ipstr[INET6_ADDRSTRLEN];
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
        inet_ntop(res->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }

    // make a socket:

    printf("Before making socket 2\n");
    sockfd2 = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    printf("After making socket2\n");

    // connect!
    int connStatus;
    if ((connStatus = connect(sockfd2, res->ai_addr, res->ai_addrlen)) != 0) {
        fprintf(stderr, "connect error: %s\n", gai_strerror(status));
        exit(1);
    } else {
        printf("Connect success\n");
        
    }

    freeaddrinfo(res);

    
    
    return 0;
}