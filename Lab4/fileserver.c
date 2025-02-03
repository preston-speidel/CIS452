#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int running = 1;  // Flag to control the main loop
int timeWaited = 0; // Keep track of all the time threads have waited

void *print_string(void *arg);
int closeProgram(pthread_t *threads, int threadC);
void handler(int num);

int main() {
    // Where the ids of the newly created threads will be stored 
    pthread_t *threads = NULL;  
    int threadCount = 0;
    int maxThreads = 10;  // The amount of threads we anticipate needing
    char input[256];

    //signal handler
    signal(SIGINT, handler);

    // Allocating space for the thread to exist
    threads = malloc(maxThreads * sizeof(pthread_t)); 
    if (threads == NULL) {
        perror("Failed to allocate memory for threads");
        return 1;
    }

    while (running) {
        printf("Enter a string (type 'exit' or press Ctrl+C & press enter to quit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline

        char *input_copy = strdup(input);
        if (input_copy == NULL) {
            perror("Invalid input try again");
            break;
        }
        
        if (strcmp(input, "exit") == 0) {
            free(input_copy); //free allocated space for the input
            running = 0;  // Exit the loop
            break;
        }

        // Check if we need to reallocate space for the thread array
        if (threadCount >= maxThreads) {
            maxThreads *= 2;
            pthread_t *new_threads = realloc(threads, maxThreads * sizeof(pthread_t));
            if (new_threads == NULL) {
                perror("Failed to reallocate threads");
                free(input_copy); //free allocated space for the input
                running = 0;  // finish all of the threads and exit
                break; 
            }
            threads = new_threads;
        }

        // Create the thread and store its ID
        if (pthread_create(&threads[threadCount], NULL, print_string, (void *)input_copy) != 0) {
            perror("Failed to create thread");
            free(input_copy);
            continue;
        }

        threadCount++;
    }

    // Wait for all threads to finish and clean up
    closeProgram(threads, threadCount);
    return 0;
}

void *print_string(void *arg) {
    char *str = (char *)arg;
    int random_value = rand() % 100;  // Generates a number between 0-99

    if (random_value < 80) {  // 80% chance
        timeWaited += 1; 
        if (sleep(1) != 0) {
            perror("Sleep interrupted");
        }
    } else {  // 20% chance
        timeWaited += 8;
        if (sleep(8) != 0) {
            perror("Sleep interrupted");
        }
    }

    printf("\n%s\n", str);  // Print the string to the console
    
    // Free the allocated string
    free(str);
    return NULL;
}

int closeProgram(pthread_t *threads, int threadC) {
    // Join all threads waiting for each to execute before exiting
    for (int i = 0; i < threadC; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    // Free the allocated space for the thread id array 
    free(threads);

    // Print the total thread count to the console
    printf("Threads created: %d\n", threadC);
    printf("Total Time threads waited: %d\n", timeWaited);

    return 0;
}

void handler(int num){
    /* 
    for now this handles the signal for ^c. The strange thing is that after 
    pressing ^c the user has to press enter. This is because of the scanf 
    makes the terminal wait until the user inputs ENTER.
    */
    running = 0;
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int running = 1;  // Flag to control the main loop
int timeWaited = 0; // Keep track of all the time threads have waited

void *print_string(void *arg);
int closeProgram(pthread_t *threads, int threadC);
void handler(int num);


int main() {
    // Where the ids of the newly created threads will be stored 
    pthread_t *threads = NULL;  
    int threadCount = 0;
    int maxThreads = 10;  // The amount of threads we anticipate needing
    char input[256];

    //signal handler
    signal(SIGINT, handler);

    // Allocating space for the thread to exist
    threads = malloc(maxThreads * sizeof(pthread_t)); 
    if (threads == NULL) {
        perror("Failed to allocate memory for threads");
        return 1;
    }

    // Clear the output file
    FILE *file = fopen("output.txt", "w");
    if (file) fclose(file); 

    while (running) {
        printf("Enter a string (type 'exit' or press Ctrl+C & press enter to quit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;  // Remove newline

        char *input_copy = strdup(input);
        if (input_copy == NULL) {
            perror("Invalid input try again");
            break;
        }
        
        if (strcmp(input, "exit") == 0) {
            free(input_copy); //free alloacted space for the input
            running = 0;  // Exit the loop
            break;
        }

        // Check if we need to reallocate space for the thread array
        if (threadCount >= maxThreads) {
            maxThreads *= 2;
            pthread_t *new_threads = realloc(threads, maxThreads * sizeof(pthread_t));
            if (new_threads == NULL) {
                perror("Failed to reallocate threads");
                free(input_copy); //free alloacted space for the input
                running = 0;  // finish all of the threads and exit
                break; 
            }
            threads = new_threads;
        }

        // Create the thread and store its ID
        if (pthread_create(&threads[threadCount], NULL, print_string, (void *)input_copy) != 0) {
            perror("Failed to create thread");
            free(input_copy);
            continue;
        }

        threadCount++;
    }

    // Wait for all threads to finish and clean up
    closeProgram(threads, threadCount);
    return 0;
}

void *print_string(void *arg) {
    char *str = (char *)arg;
    FILE *file = fopen("output.txt", "a");  // Open file in append mode

    int random_value = rand() % 100;  // Generates a number between 0-99

    if (random_value < 80) {  // 80% chance
        timeWaited += 1; 
        if (sleep(1) != 0) {
            perror("Sleep interrupted");
        }
    } else {  // 20% chance
        timeWaited += 8;
        if (sleep(8) != 0) {
            perror("Sleep interrupted");
        }
    }

    if (file == NULL) {
        perror("Error opening file");
        free(str); // Free the allocated string even if the file couldn't be opened.
        return NULL;
    }
    fprintf(file, "%s\n", str);  // Write the string to the file
    fclose(file);  // Close the file after writing
    
    // Free the allocated string
    free(str);
    return NULL;
}

int closeProgram(pthread_t *threads, int threadC) {
    // Join all threads waiting for each to execute before exiting
    for (int i = 0; i < threadC; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    // Free the allocated space for the threadid array 
    free(threads);

    // Write the total thread count to the file
    FILE *file = fopen("output.txt", "a");
    if (file) {
        fprintf(file, "Threads created: %d\n", threadC);
        fprintf(file, "Total Time threads waited: %d", timeWaited);
        fclose(file);
    }

    return 0;
}


void handler(int num){
    //for now this handles the signal for ^c. The strange this is that after 
    //pressing ^c the user has to press enter. This is because of the scanf 
    //makes the terminal wait until the user inputs ENTER.
    
    running = 0;
}
*/