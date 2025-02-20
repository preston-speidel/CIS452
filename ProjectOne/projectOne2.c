#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1


int numProcess;
pid_t *pids;  // Array of child process IDs
int (*pipes)[2];  // Array of pipes
volatile sig_atomic_t interrupt = 1;  // Flag to control the parent loop

typedef struct {
    int destination;
    char message[50];
} Apple;

void sigint_handler(int sig);

int main() {
    printf("How many nodes do you want to create? ");
    if (scanf("%d", &numProcess) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    
    pipes = malloc(numProcess * sizeof(int[2]));// Allocate space for each pipe
    pids = malloc((numProcess - 1) * sizeof(pid_t));  // Allocate space for child process IDs

    // Create pipes for all nodes
    for (int i = 0; i < numProcess; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    // Fork to create child process nodes
    for (int i = 0; i < numProcess - 1; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pids[i] == 0) { // Child process 
            int myId = i + 1;  // first child starts at 1
            int nextId = (myId + 1) % numProcess; // make it so we wrap back around
            int prevId = (myId - 1 + numProcess) % numProcess;
            
            //loop to keep pipe ends open
            while (1) {
                Apple aMessage;

                for (int j = 0; j < numProcess; j++) {
                    if (j != myId) {
                        close(pipes[j][WRITE]); // Keep only current node WRITE open
                    }; 
                    if (j != prevId) {
                        close(pipes[j][READ]); // Keep only previous node's READ open
                    };  
                }

                // Read from previous node
                read(pipes[prevId][READ], &aMessage, sizeof(Apple));

                // Determine if the message is intended for this node
                if (aMessage.destination == myId) {
                    printf("Node %d (PID %d) received message and it does belong\n", myId, getpid());
                    printf("\nMessage = %s\n", aMessage.message);
                    strcpy(aMessage.message, "Empty\n");
                    write(pipes[myId][WRITE], &aMessage, sizeof(Apple));
                } else {
                    printf("Node %d (PID %d) received message and does not belong:\n", myId,  getpid());
                    printf("Node %d sending message to Node %d\n", myId, nextId);
                    write(pipes[myId][WRITE], &aMessage, sizeof(Apple));
                }
            }
        }
    }

    // Parent process acts is Node 0 
    int myId = 0;
    int nextId = (myId + 1) % numProcess;
    int prevId = (myId - 1 + numProcess) % numProcess;

    // Close unused pipe ends
    for (int j = 0; j < numProcess; j++) {
        if (j != myId){
        close(pipes[j][WRITE]);  // Keep only Node 0's WRITE open
        }
        if (j != prevId){
             close(pipes[j][READ]);  // Keep only last node's READ open
        }
    }

    // signal handler for ^c
    signal(SIGINT, sigint_handler);

    Apple aMessage;

    //loop to keep pipe ends open
    while (interrupt) {
        printf("Enter the Destination of the message: ");
        scanf("%d", &aMessage.destination);

        // Clear the buffer to remove leftover newline
        while (getchar() != '\n');

        printf("Enter in a message to be passed: ");
        fgets(aMessage.message, sizeof(aMessage.message), stdin);

        // Node 0 starts message sending process
        write(pipes[myId][WRITE], &aMessage, sizeof(Apple));
        printf("\nNode %d (PID %d) sent message: %s", myId, getpid(), aMessage.message);

        // Read from last node
        read(pipes[prevId][READ], &aMessage, sizeof(Apple));
        printf("Message arrived back to node %d (PID %d) message: %s\n", myId, getpid(),aMessage.message);
    }
    
    return 0;
}


// Signal handler for SIGINT
void sigint_handler(int sig) {
    printf("\n Exiting the loop program\n");
    interrupt = 0;  // Set the flag to exit the loop 
    // Close all pipe ends
    for (int i = 0; i < numProcess; i++) {
        printf("Closing READ end of pipe %d in signal handler\n", i);
        close(pipes[i][READ]);
        printf("Closing WRITE end of pipe %d in signal handler\n", i);
        close(pipes[i][WRITE]);
    }

    // Kill all child processes and print their PIDs
    for (int i = 0; i < numProcess - 1; i++) {
        printf("Killing child PID: %d\n", pids[i]);
        kill(pids[i], SIGTERM);
    }

    // Free allocated memory
    free(pipes);
    free(pids);

    printf("\nAll processes terminated. Exiting now.\n");
    exit(0);
}