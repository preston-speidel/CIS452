#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    // Start with 1MB
    size_t size = 1024 * 1024; 
    int shmid;

    printf("Determining maximum shared memory segment size...\n");

    while (1) {
        shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
        if (shmid == -1) {
            printf("Maximum shared memory segment size found: %zu bytes\n", size / 1024 / 1024);
            break;
        } else {
            // Remove segment
            shmctl(shmid, IPC_RMID, NULL); 
        }
        // Increase by 1MB
        size += 1024 * 1024; 
    }

    return 0;
}