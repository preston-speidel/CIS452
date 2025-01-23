#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2]; 
    int pipeCreationResult;
    pid_t pid;
    char inputString[500]; 
    char buffer[500]; 

    // Create the pipe
    pipeCreationResult = pipe(fd);
    if (pipeCreationResult < 0) {
        perror("Failed pipe creation\n");
        exit(1);
    }

    // Fork the process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child process
        // Close read end
        close(fd[0]); 
        printf("Child: Please enter a message (up to 500 characters): ");
        // Get input from user
        fgets(inputString, sizeof(inputString), stdin); 
        // Send message to pipe
        write(fd[1], inputString, strlen(inputString) + 1); 
        printf("Child: Sent message: %s", inputString);
        // Close write end
        close(fd[1]); 
    } else {
        // Parent process
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        printf("Parent: Received message: %s", buffer);
        close(fd[0]);
    }
    return 0;
}