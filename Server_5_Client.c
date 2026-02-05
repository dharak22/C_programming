#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX_CLIENTS 5

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_no = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, MAX_CLIENTS);
    printf("Server started. Waiting for clients...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        client_no++;
        if (client_no > MAX_CLIENTS) {
            close(client_fd);
            continue;
        }

        if (fork() == 0) {  // Child process
            char buffer[1024];
            int bytes;

            printf("Client %d connected\n", client_no);

            while ((bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
                buffer[bytes] = '\0';
                printf("Client %d: %s\n", client_no, buffer);
            }

            printf("Client %d terminated\n", client_no);
            close(client_fd);
            exit(0);
        }

        close(client_fd);  // Parent closes client socket
    }

    close(server_fd);
    return 0;
}
