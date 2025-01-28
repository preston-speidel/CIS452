#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define READ 0
#define WRITE 1

int main(){
    int fd[2];
    int pipeCreationResult;
    int pid;

    pipeCreationResult = pipe(fd);

    if(pipeCreationResult < 0){
        perror("Failed pipe creation\n");
        exit(1);
    }

    pid = fork();

    if(pid < 0){ // Fork failed
        perror("Fork failed");
        exit(1);
    }

    int output = 3;
    int input;

    char myStringOutput[] = "This a test!";
    char myStringInput[50];

    if(pid == 0){
        // Child process
        // Close unused read end
        close(fd[READ]); 
        // Write the string
        write(fd[WRITE], myStringOutput, strlen(myStringOutput) + 1); 
        printf("Child wrote: %s\n", myStringOutput);
        // Close the write end
        close(fd[WRITE]); 
    }
    else{
        // Close unused write end
        close(fd[WRITE]); 
        // Read the string
        ssize_t bytesRead = read(fd[READ], myStringInput, sizeof(myStringInput)); 
        printf("Parent received: %s from child process (Bytes read: %ld)\n", myStringInput, bytesRead);
        // Close the read end
        close(fd[READ]); 
    }
    
    return 0;
}