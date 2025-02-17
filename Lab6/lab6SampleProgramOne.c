// Created by Preston Speidel and Devonte Hillman with the assistance of chatgpt

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

// Functions to perform semaphore operations (wait and signal)
void sem_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    semop(sem_id, &sb, 1);
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0}; 
    semop(sem_id, &sb, 1);
}

int main(int argc, char *argv[])
{
    int status;
    long int i, loop, temp, *sharedMemoryPointer;
    int sharedMemoryID;
    // Semaphore ID
    int sem_id;  
    pid_t pid;

    loop = atoi(argv[1]);

    // Create shared memory
    sharedMemoryID = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(sharedMemoryID < 0) {
        perror("Unable to obtain shared memory");
        exit(1);
    }

    // Attach shared memory
    sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);
    if(sharedMemoryPointer == (void*) -1) {
        perror("Unable to attach");
        exit(1);
    }

    sharedMemoryPointer[0] = 0;
    sharedMemoryPointer[1] = 1;

    // Create the semaphore
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if(sem_id < 0) {
        perror("Unable to create semaphore");
        exit(1);
    }

    // Initialize the semaphore to 1 (available)
    if(semctl(sem_id, 0, SETVAL, 1) < 0) {
        perror("Unable to initialize semaphore");
        exit(1);
    }

    pid = fork();
    if(pid < 0){
        printf("Fork failed\n");
    }

    // Child Process
    if(pid == 0) { 
        for(i = 0; i < loop; i++) {
            // Lock before swapping
            sem_wait(sem_id);  
            temp = sharedMemoryPointer[0];
            sharedMemoryPointer[0] = sharedMemoryPointer[1];
            sharedMemoryPointer[1] = temp;
            // Unlock after swapping
            sem_signal(sem_id);  
        }
        if(shmdt(sharedMemoryPointer) < 0) {
            perror("Unable to detach");
            exit(1);
        }
        exit(0);
    } 
    // Parent Process
    else { 
        for(i = 0; i < loop; i++) {
            // Lock before swapping
            sem_wait(sem_id);  
            temp = sharedMemoryPointer[1];
            sharedMemoryPointer[1] = sharedMemoryPointer[0];
            sharedMemoryPointer[0] = temp;
            // Unlock after swapping
            sem_signal(sem_id);  
        }
    }

    wait(&status);
    printf("Values: %li\t%li\n", sharedMemoryPointer[0], sharedMemoryPointer[1]);

    // Detach shared memory
    if(shmdt(sharedMemoryPointer) < 0) {
        perror("Unable to detach");
        exit(1);
    }

    // Remove shared memory
    if(shmctl(sharedMemoryID, IPC_RMID, 0) < 0) {
        perror("Unable to deallocate");
        exit(1);
    }

    // Remove the semaphore
    if(semctl(sem_id, 0, IPC_RMID) < 0) {
        perror("Unable to remove semaphore");
        exit(1);
    }

    return 0;
}