#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8888

void printArray(int *arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main() {
    int sock;
    struct sockaddr_in server, client;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running, enter the array at client side...\n");

    while (1) {
        socklen_t addr_len = sizeof(client);

        int array_size;
        if (recvfrom(sock, &array_size, sizeof(array_size), 0, (struct sockaddr *)&client, &addr_len) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        int client_data[array_size];
        if (recvfrom(sock, &client_data, array_size * sizeof(int), 0, (struct sockaddr *)&client, &addr_len) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < array_size - 1; i++) {
            for (int j = 0; j < array_size - i - 1; j++) {
                if (client_data[j] > client_data[j + 1]) {
                    int temp = client_data[j];
                    client_data[j] = client_data[j + 1];
                    client_data[j + 1] = temp;
                }
            }

            printf("Iteration %d: ", i+1);
            printArray(client_data, array_size);

            // Send the array after each iteration to the client
            if (sendto(sock, &client_data, array_size * sizeof(int), 0, (struct sockaddr *)&client, addr_len) == -1) {
                perror("sendto");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(sock);
    return 0;
}

