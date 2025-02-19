// assistance from chatgpt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_MESSAGE_LEN 100
#define MAX_NODES       100

// We'll have k pipes (one per link in the ring).
// pipeArray[i][0] = read end  for link i
// pipeArray[i][1] = write end for link i
static int pipeArray[MAX_NODES][2];

int parentPid;
int nodeCount;

// Handle Ctrl+C in the parent
void handleSIGINT(int sig) {
    if (getpid() == parentPid) {
        printf("\n[Parent] Terminating...\n");
        exit(0);
    }
}

// Each child node i (1..k-1) runs here
void runChildNode(int nodeID) {
    // read from pipeArray[nodeID-1][0] and write to pipeArray[nodeID][1]
    // consider wrapping for node 0..(k-1). The read link is (nodeID-1 + k) % k.
    int readLink = (nodeID - 1 + nodeCount) % nodeCount;
    int writeLink = nodeID; // node i writes to link i

    // Close all pipe ends this node doesn't need
    for (int j = 0; j < nodeCount; j++) {
        if (j != readLink) {
            close(pipeArray[j][0]); // not reading from these
        }
        if (j != writeLink) {
            close(pipeArray[j][1]); // not writing to these
        }
    }

    while (1) {
        char buffer[MAX_MESSAGE_LEN];
        // Read message from our readLink
        int bytes = read(pipeArray[readLink][0], buffer, MAX_MESSAGE_LEN);
        if (bytes <= 0) {
            fprintf(stderr, "[Node %d] Error reading, exiting.\n", nodeID);
            exit(1);
        }

        printf("[Node %d] Received: %s\n", nodeID, buffer);
        fflush(stdout);

        // If it's not "empty", parse destination
        if (strcmp(buffer, "empty") != 0) {
            int destination;
            sscanf(buffer, "%d:", &destination);
            // If we are the destination node
            if (destination == nodeID) {
                printf("[Node %d] Message received: %s\n", nodeID, buffer + 2);
                fflush(stdout);
                // Mark the message empty
                strcpy(buffer, "empty");
            }
        }

        // Forward to next node in the ring
        int nextNode = (nodeID + 1) % nodeCount;
        printf("[Node %d] Forwarding to Node %d\n", nodeID, nextNode);
        fflush(stdout);
        write(pipeArray[nodeID][1], buffer, MAX_MESSAGE_LEN);
    }
}

int main() {
    parentPid = getpid();
    signal(SIGINT, handleSIGINT); // Catch Ctrl+C

    printf("Enter number of nodes (k): ");
    fflush(stdout);

    // Read nodeCount
    if (scanf("%d", &nodeCount) != 1 || nodeCount < 2 || nodeCount > MAX_NODES) {
        fprintf(stderr, "Invalid node count.\n");
        return 1;
    }

    // Create k pipes
    for (int i = 0; i < nodeCount; i++) {
        if (pipe(pipeArray[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    // Fork child processes for nodes 1..(k-1)
    for (int i = 1; i < nodeCount; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            runChildNode(i);
            exit(0);
        }
    }

    // Parent = node 0
    // write to pipeArray[0][1], read from pipeArray[k-1][0]
    // Close everything else
    for (int j = 1; j < nodeCount - 1; j++) {
        close(pipeArray[j][0]);
        close(pipeArray[j][1]);
    }
    close(pipeArray[0][0]);           // don't need read end of link 0
    if (nodeCount > 1) close(pipeArray[nodeCount - 1][1]); // don't write to link k-1

    // Clear leftover newline from the scanf
    int dummy;
    while ((dummy = getchar()) != '\n' && dummy != EOF) {}

    // Repeatedly ask for messages
    while (1) {
        char inputMsg[MAX_MESSAGE_LEN];
        printf("\n[Parent] Enter message: ");
        fflush(stdout);

        // Read a full line
        if (!fgets(inputMsg, sizeof(inputMsg), stdin)) {
            printf("[Parent] EOF or error reading input. Exiting.\n");
            break;
        }

        // Strip trailing newline
        size_t len = strlen(inputMsg);
        if (len > 0 && inputMsg[len - 1] == '\n') {
            inputMsg[len - 1] = '\0';
        }
        if (strlen(inputMsg) == 0) {
            printf("[Parent] Empty message, try again.\n");
            continue;
        }

        // Destination node
        printf("[Parent] Enter destination node (0-%d): ", nodeCount - 1);
        fflush(stdout);
        int destination;
        if (scanf("%d", &destination) != 1 || destination < 0 || destination >= nodeCount) {
            printf("[Parent] Invalid destination.\n");
            // flush leftover
            while ((dummy = getchar()) != '\n' && dummy != EOF) {}
            continue;
        }
        while ((dummy = getchar()) != '\n' && dummy != EOF) {}

        // Format: "destination:body" 
        // the destination is within the message so it knows where it is going
        char message[MAX_MESSAGE_LEN];
        snprintf(message, sizeof(message), "%d:%s", destination, inputMsg);

        // Send to Node 1
        printf("[Parent] Sending message to Node 1\n");
        fflush(stdout);
        write(pipeArray[0][1], message, MAX_MESSAGE_LEN);

        // Wait for it to return from Node (k-1)
        char returned[MAX_MESSAGE_LEN];
        int bytesRead = read(pipeArray[nodeCount - 1][0], returned, MAX_MESSAGE_LEN);
        if (bytesRead <= 0) {
            printf("[Parent] No message returned. Exiting.\n");
            break;
        }

        printf("[Parent] Apple returned: %s\n", returned);
        fflush(stdout);
    }

    return 0;
}