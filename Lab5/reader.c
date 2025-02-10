#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 2048

int shm_id;
char *shared_mem;
int *read_flags;
int reader_id; // (0 or 1)

void cleanup(int signum) {
    shmdt(shared_mem);
    printf("\nDetached from shared memory. Exiting.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <reader_id (0 or 1)>\n", argv[0]);
        exit(1);
    }

    // Get the reader ID from command line argument
    reader_id = atoi(argv[1]); 
    if (reader_id != 0 && reader_id != 1) {
        fprintf(stderr, "Reader ID must be 0 or 1.\n");
        exit(1);
    }

    // Get the same shared memory key
    key_t key = ftok("shared_memory", 65); 
    // Attach to existing shared memory
    shm_id = shmget(key, SHM_SIZE, 0666); 
    shared_mem = (char *)shmat(shm_id, NULL, 0);
    // Use second half of shared memory for read flags
    read_flags = (int *)(shared_mem + 1024); 

    // Control c for cleanup
    signal(SIGINT, cleanup); 

    while (1) {
        // Wait for a new message
        while (read_flags[reader_id] == 1) {
            sleep(1);
        }

        // Print the message
        printf("Reader %d received: %s", reader_id, shared_mem);

        // Marks message as read
        read_flags[reader_id] = 1;
    }

    cleanup(0);
}