// De
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h> 

void trim_trailing_spaces(char *str) {
    // Removes trailing spaces or tabs
    int length = strlen(str);
    while (length > 0 && (str[length - 1] == ' ' || str[length - 1] == '\t')) {
        str[length - 1] = '\0'; // Replace trailing spaces/tabs with null terminator
        length--;
    }
}

int main() {
    const char *exit_command = "exit"; // Command to exit the program
    const char *clear_command = "clear";
    
    //vars for dynamic string input
    char *input = NULL;
    char *input_copy;
    char *token;
    char *tokens[75]; // Array to hold tokens
    int i = 0; //counter

    //clear the terminal to give an immersive experience
    system("clear");
    printf("Welcome to the Almighty Shell!\n");
    printf("Type 'exit' to quit.\n");

    while (1) { // Infinite loop to continuously prompt the user
        printf("~ "); // Shell-like prompt
        i = 0; // Reset token counter
        
        // Allocate memory for the input string
        input = malloc(100);
        fgets(input, 100, stdin); // Get user input (supports spaces)
        

        // Remove trailing newline character from input
        input[strcspn(input, "\n")] = 0;

        // Remove trailing spaces/tabs
        trim_trailing_spaces(input);

        //create copy of string to format for the exe call
        input_copy = strdup(input);
        if (input_copy == NULL) {
            perror("strdup");
            free(input);
            return 1;
        }
        // Tokenize and store each token
        while ((token = strsep(&input_copy, " ")) != NULL) {
            if (*token != '\0') { // Skip empty tokens (caused by consecutive spaces)
                tokens[i++] = token;
            }
        }
        //reallocate the memory used
        free(input);
        tokens[i] = NULL; // Null-terminate the tokens array
        

        // Check if the user wants to exit
        if (strcmp(tokens[0], exit_command) == 0) {
            printf("Exiting shell...\n");
            //clear input
            memset(tokens, '\0', sizeof(tokens));
            break;
        }

        // Create a child process using fork()
        pid_t pid, child;
        int status;


        if ((pid = fork()) < 0) {
            // Error in forking
            perror("fork failed");
            return 1;
        }

        if (pid == 0) {
            // Child process - execute the command
            if (execvp(tokens[0], tokens) == -1) {
                perror("execvp failed");
                return 1;
            }
        } else {
            // Wait for the child process to finish
            child = wait(&status);
            // Continue your main program here
            if (child > 0) {
                // Collect resource usage statistics for the child process
                struct rusage usage;
                if (getrusage(RUSAGE_CHILDREN, &usage) == 0) {
                    printf("\nclareUser CPU time used: %ld.%06ld seconds\n",
                        usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
                    printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
                } else {
                    perror("getrusage failed");
                }
            } else {
                perror("wait failed");
            }
            printf("Main program running\n");
        }
        //clear input fo new input
        memset(tokens, '\0', sizeof(tokens));
    }

    return 0;
}
