#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    const char *hello = "Hello from client";
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    //memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
//    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("192.168.10.53");

    int n;
    socklen_t len = sizeof(servaddr);

    sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';

    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}

