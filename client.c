#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MYPORT "3490"  // the port users will be connecting to
#define MAXDATASIZE 100 // max data size 

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
    struct sockaddr_storage their_addr;
     socklen_t addr_size;
     struct addrinfo hints;
     struct addrinfo* res;
     int sockfd, new_fd, numbytes;
     char s[INET6_ADDRSTRLEN];
     char buf[MAXDATASIZE];

     memset(&hints, 0, sizeof hints);
     hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
     hints.ai_socktype = SOCK_STREAM;
     hints.ai_flags = AI_PASSIVE;

     getaddrinfo(NULL, MYPORT, &hints, &res);
     
     sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //  bind(sockfd, res->ai_addr, res->ai_addrlen);

    connect(sockfd, res->ai_addr, res->ai_addrlen);

    // inet_ntop(res->ai_family, (struct sockaddr *)res->ai_addr->)

    inet_ntop(res->ai_family, get_in_addr((struct sockaddr *)res->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);

    const char* msg = "Hello World";
    int len, bytes_sent;

    len = strlen(msg);
    bytes_sent = send(sockfd, msg, len, 0);
    if (bytes_sent == -1) {
          printf("error sending bytes \n");
    } else {
          printf("Send data\n");
    }

    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(sockfd);
    
    return 0;
}