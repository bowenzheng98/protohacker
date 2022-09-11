#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

int main(void) {
     struct sockaddr_storage their_addr;
     socklen_t addr_size;
     struct addrinfo hints;
     struct addrinfo* res;
     int sockfd, new_fd;

     memset(&hints, 0, sizeof hints);
     hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
     hints.ai_socktype = SOCK_STREAM;
     hints.ai_flags = AI_PASSIVE;

     getaddrinfo(NULL, MYPORT, &hints, &res);
     
     sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
     if (sockfd == -1) {
          printf("socket error \n");
          return 1;
     } else {
          printf("Created socket \n");
     }

     if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
          printf("Socket bind error \n");
          return 1;
     } else {
          printf("Socket bound \n");
     }


     if (listen(sockfd, BACKLOG) == -1)  {
          printf("Socket error on listen \n");
          return 1;
     } else {
          printf("Listening on socket \n");
     }

     addr_size = sizeof their_addr;
     new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

     printf("Got connection \n");

     const char* msg = "Hello World";
     int len, bytes_sent;

     len = strlen(msg);
     bytes_sent = send(new_fd, msg, len, 0);
     if (bytes_sent == -1) {
          printf("error sending bytes \n");
     } else {
          printf("Send data\n");
     }

     close(new_fd);
     close(sockfd);
     printf("Closing connection");
 
     return 0;
}