#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2]; // Array to store pipe ends
    int pipeCreationResult;
    pid_t pid;
    char inputString[500]; // Buffer for input
    char buffer[500]; // Buffer for reading from the pipe

    // Create the pipe
    pipeCreationResult = pipe(fd);
    if (pipeCreationResult < 0) {
        perror("Failed pipe creation\n");
        exit(1);
    }

    // Fork a process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(fd[0]); // Close read end
        printf("Child: Please enter a message (up to 500 characters): ");
        fgets(inputString, sizeof(inputString), stdin); // Get input from user
        write(fd[1], inputString, strlen(inputString) + 1); // Send message to pipe
        printf("Child: Sent message: %s", inputString);
        close(fd[1]); // Close write end
    } else {
        // Parent process
        close(fd[1]); // Close write end
        read(fd[0], buffer, sizeof(buffer)); // Read message from pipe
        printf("Parent: Received message: %s", buffer);
        close(fd[0]); // Close read end
    }

    return 0;
}