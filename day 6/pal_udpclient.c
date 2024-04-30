#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd, n, flag;
    char buffer[10];
    struct sockaddr_in server;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
    server.sin_addr.s_addr = inet_addr("192.168.10.53"); // Change to the server's IP address

    printf("Enter a string to check palindrome: ");
    scanf("%s", buffer);

    // Send the string to the server
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, sizeof(server));

    // Receive the result from the server
    recvfrom(sockfd, &flag, sizeof(int), 0, NULL, NULL);

    if (flag == 1)
        printf("Server: The string is a palindrome.\n");
    else
        printf("Server: The string is not a palindrome.\n");

    close(sockfd);
    return 0;
}
