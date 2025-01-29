#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>


// write to 1 and read from 0
#define READ 0
#define WRITE 1

void sigHandler1(int sigNum);
void sigHandler2(int sigNum);
void sigHandler3(int sigNum);


int main() {
    pid_t pid, child_pid;

    // Seed the random number generator with the current time
    srand(time(NULL));

    if ((pid = fork()) < 0) { // Fork failed
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) { // Child process
        //printf("spawned child PID# %d\n", getpid());

        while(1){
            //Generate a random number between 1 and 5 
            sleep((rand() % 5) + 1);
            
            // Generate a random number between 1 and 2
            if(rand() % 2 == 0){
                printf("waiting...");
                fflush(stdout); // Flush the output buffer
                kill(getppid(), SIGUSR1);
            }else{
                printf("waiting...");
                fflush(stdout); // Flush the output buffer
                kill(getppid(), SIGUSR2);
            }
        }
        return 0;
    } else { // Parent process
        // In the parent proccess fork returns child pid 
        child_pid = pid;
        printf("spawned child PID# %d\n", child_pid);
    
        signal(SIGUSR1, sigHandler1);//signal handler for the two user defined signals
        signal(SIGUSR2, sigHandler2);
        signal(SIGINT,  sigHandler3);
        
        wait(NULL);// wait for the child to execute
    }

    return 0;
}


void sigHandler1(int sigNum) {
    printf("\t\treceived a SIGUSR1 signal\n");
    return;
}

void sigHandler2(int sigNum) {
    printf("\t\treceived a SIGUSR2 signal\n");
    // this is where shutdown code would be inserted
    //Send this signal to the parent 
    return;
}

void sigHandler3(int sigNum) {
    printf(" received. That's it, I'm shutting you down...\n"); 
    return;
}