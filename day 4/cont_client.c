// TCP Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, PORT);

    // Continuous communication
    while (1) {
        // Get user input
        printf("Enter message to send (or 'exit' to quit): ");
        fgets(buffer, sizeof(buffer), stdin);

        // Check for exit command
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        // Send the message to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive and display the server's response
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            // Connection closed or error occurred
            printf("Connection closed by server\n");
            break;
        }

        // Null-terminate the received data
        buffer[bytes_received] = '\0';

        // Display server's response
        printf("Server response: %s", buffer);
    }

    // Close the client socket
    close(client_socket);

    return 0;
}

