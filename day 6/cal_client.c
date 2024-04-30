#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT  4000
#define MAXLINE  1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM,  0)) <  0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr,  0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr(""); 
    servaddr.sin_port = htons(SERVER_PORT);

    printf("\nSelect an operation:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Division\n");
    printf("4. Multiplication\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");

    printf("Enter the first number, and the second number followed by operation number:\n");
	
	
    fgets(buffer, MAXLINE, stdin);
    buffer[strcspn(buffer, "\n")] =  0; // Remove trailing newline

  
    if (sendto(sockfd, buffer, strlen(buffer),  0, (const struct sockaddr *)&servaddr, sizeof(servaddr)) <  0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    
    socklen_t len = sizeof(servaddr);
    if (recvfrom(sockfd, buffer, MAXLINE,  0, (struct sockaddr *)&servaddr, &len) <  0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    
    printf("Server reply: %s\n", buffer);

    close(sockfd);
    return  0;
}
