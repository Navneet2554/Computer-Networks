#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    int array_size;
    printf("Enter the size of the array: ");
    scanf("%d", &array_size);

    int array[array_size];
    printf("Enter %d elements for the array:\n", array_size);
    for (int i = 0; i < array_size; ++i) {
        scanf("%d", &array[i]);
    }

    send(client_socket, &array_size, sizeof(array_size), 0);
    send(client_socket, array, sizeof(array), 0);

    int sum;
    recv(client_socket, &sum, sizeof(sum), 0);

    printf("Sum of the array elements received from the server: %d\n", sum);

    close(client_socket);

    return 0;
}
