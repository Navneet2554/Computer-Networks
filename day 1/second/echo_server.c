#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void start_server() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening for incoming connections...\n");

    while (1) {
        // Accept a connection
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len)) == -1) {
            perror("Acceptance failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection established with %s\n", inet_ntoa(client_address.sin_addr));

        // Receive data from the client
        ssize_t received_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (received_bytes == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        buffer[received_bytes] = '\0';
        printf("Received message from client: %s\n", buffer);

        // Send the received message back to the client
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        printf("Sent message back to client\n");

        // Close the connection
        close(client_socket);
    }
}

int main() {
    start_server();
    return 0;
}
