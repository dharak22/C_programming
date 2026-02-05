#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];          // fd[0] = read end, fd[1] = write end
    char buffer[100];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create a child process
    if (fork() == 0) {
        // Child process
        close(fd[1]);   // Close write end

        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(fd[0]);
    } else {
        // Parent process
        close(fd[0]);   // Close read end

        char msg[] = "Hello from parent!";
        write(fd[1], msg, strlen(msg) + 1);

        close(fd[1]);
    }

    return 0;
}
