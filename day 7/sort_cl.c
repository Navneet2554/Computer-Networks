#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
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
    struct sockaddr_in server;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(PORT);
    int array_size;
    printf("Enter the size of the array: ");
    scanf("%d", &array_size);
    if (sendto(sock, &array_size, sizeof(array_size), 0, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("sendto");
        return -1;
    }
    int client_data[array_size];
    printf("Enter %d integers for the array:\n", array_size);
    for (int i = 0; i < array_size; i++) {
        scanf("%d", &client_data[i]);
    }
    if (sendto(sock, &client_data, array_size * sizeof(int), 0, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("sendto");
        return -1;
    }

    puts("Unsorted array : ");
    printArray(client_data, array_size);

    for (int i = 0; i < array_size - 1; i++) {
        int server_reply[array_size];
        if (recvfrom(sock, &server_reply, array_size * sizeof(int), 0, NULL, NULL) == -1) {
            perror("recvfrom");
            return -1;
        }

        printf("Iteration %d: ", i+1);
        printArray(server_reply, array_size);
    }

    close(sock);
    return 0;
}

