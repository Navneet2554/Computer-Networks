#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT  4000
#define MAXLINE  1024


double performOperation(double num1, double num2, char operation);

int main() {
    int sockfd,operationNum;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    

    int choice;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM,  0)) <  0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
   // servaddr.sin_addr.s_addr = inet_addr("192.168.10.44");
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) <  0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t len;
    int n;
    len = sizeof(cliaddr);
 do {
        // Receive a message from the client
        n = recvfrom(sockfd, (char *)buffer, MAXLINE,  0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        
        double num1, num2;
        int operationNum;
        sscanf(buffer, "%lf %lf %d", &num1, &num2, &operationNum);

        
        char operation;
        switch (operationNum) {
            case  1: operation = '+'; break;
            case  2: operation = '-'; break;
            case  3: operation = '/'; break;
            case  4: operation = '*'; break;
            default: operation = '?'; // Invalid operation number
        }

        
        double result = performOperation(num1, num2, operation);
        char resultStr[MAXLINE];
        snprintf(resultStr, MAXLINE, "Result: %.2f", result);

        
        sendto(sockfd, (const char *)resultStr, strlen(resultStr),  0, (const struct sockaddr *)&cliaddr, len);

    } while (operationNum !=  5); // Continue receiving messages until the user chooses to exit

    close(sockfd);
    return  0;
}

void displayMenu(){
    printf("\nSelect an operation:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Division\n");
    printf("4. Multiplication\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

double performOperation(double num1, double num2, char operation) {
    switch (tolower(operation)) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 !=  0 ? num1 / num2 :  0; // No division by zero
        default:  return  0; // Invalid operatn
    }
}
