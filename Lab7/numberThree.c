#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>      // For O_CREAT
#include <errno.h>      // For error handling
#include <stdlib.h>     // For exit()

int main() {
    sem_t *sem;
    int max_value = 0;

    // Create an unnamed semaphore with an initial value of 0
    sem = sem_open("/test_semaphore", O_CREAT, 0644, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        return 1;
    }

    // Keep incrementing the semaphore value until it fails
    while (sem_post(sem) == 0) {
        max_value++;
    }

    // sem_post failed, meaning we've reached the limit
    printf("Empirically determined maximum value of a counting semaphore: %d\n", max_value);

    // Cleanup
    sem_close(sem);
    sem_unlink("/test_semaphore");

    return 0;
}