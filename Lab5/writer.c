// Created by Preston Speidel and Devonte Hillman with the assistance of chatgpt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 2048

int shm_id;
char *shared_mem;
int *read_flags;

void cleanup(int signum) {
    shmdt(shared_mem);
    shmctl(shm_id, IPC_RMID, NULL);
    printf("\nShared memory removed. Exiting.\n");
    exit(0);
}

int main() {
    // Generates a unique key for shared memory
    key_t key = ftok("shared_memory", 65); 
    // Create shared memory
    shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666); 
    // Attach to shared memory
    shared_mem = (char *)shmat(shm_id, NULL, 0); 
    // Second half of shared memory for the read flags
    read_flags = (int *)(shared_mem + 1024); 

    // Control c for cleanup
    signal(SIGINT, cleanup); 

    while (1) {
        printf("Enter message (type 'quit' to exit): ");
        // Write message to first half of the shared memory
        fgets(shared_mem, 1024, stdin); 

        if (strncmp(shared_mem, "quit", 4) == 0) break;

        // Reset read flags
        read_flags[0] = 0;
        read_flags[1] = 0;

        // Wait for both of the readers to read the message
        while (read_flags[0] == 0 || read_flags[1] == 0) {
            sleep(1);
        }
    }

    cleanup(0);
}