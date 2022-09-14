#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold
#define MAXDATASIZE 100 // max data size 

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

     int bytes_recv;
     char* msg;
     if ((bytes_recv = recv(new_fd, msg, MAXDATASIZE-1, 0)) == -1) {
          perror("receive error");
          exit(1);
     }

     int bytes_sent;
     if ((bytes_sent = send(new_fd, msg, strlen(msg), 0)) == -1) {
          perror("send error");
          exit(1);
     }

     close(new_fd);
     close(sockfd);
     printf("Closing connection");
 
     return 0;
}