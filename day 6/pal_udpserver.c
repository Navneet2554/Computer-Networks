#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd, n;
    char buffer[20];
    struct sockaddr_in server, client;

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is waiting for data...\n");

    while (1) {
        socklen_t len = sizeof(client);
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &len);
        if (n == -1) {
            perror("Error receiving data");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Received string: %s\n", buffer);

        // Check if the received string is a palindrome
        int left = 0;
        int right = strlen(buffer) - 1;
        int flag = 1;

        while (left < right && flag) {
            if (buffer[left] != buffer[right])
                flag = 0;
            else {
                left++;
                right--;
            }
        }

        // Send the result back to the client
        sendto(sockfd, &flag, sizeof(int), 0, (struct sockaddr*)&client, len);
    }

    close(sockfd);
    return 0;
}
