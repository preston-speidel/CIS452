#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>

int main() {
    size_t size = 1024 * 1024; // Start with 1MB
    int shmid;

    printf("Determining maximum shared memory segment size...\n");

    while (1) {
        shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
        if (shmid == -1) {
            printf("Maximum shared memory segment size found: %zu bytes\n", size / 1024 / 1024);
            break;
        } else {
            shmctl(shmid, IPC_RMID, NULL); // Remove segment
        }
        size += 1024 * 1024; // Increase by 1MB
    }

    return 0;
}