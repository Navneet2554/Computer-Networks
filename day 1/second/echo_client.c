#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void start_client(const char* message) {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Sending message to server: %s\n", message);

    // Send message to the server
    if (send(client_socket, message, strlen(message), 0) == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive response from the server
    ssize_t received_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (received_bytes == -1) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    printf("Received response from server: %s\n", buffer);

    // Close the connection
    close(client_socket);
}

int main() {
    char user_message[BUFFER_SIZE];
    printf("Enter a message to send to the server: ");
    fgets(user_message, sizeof(user_message), stdin);

    // Remove newline character from the input
    user_message[strcspn(user_message, "\n")] = '\0';

    start_client(user_message);
    return 0;
}
