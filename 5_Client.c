#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server. Type messages (type exit to quit):\n");

    while (1) {
        fgets(message, sizeof(message), stdin);

        if (strncmp(message, "exit", 4) == 0)
            break;

        send(sock, message, strlen(message), 0);
    }

    close(sock);
    return 0;
}
