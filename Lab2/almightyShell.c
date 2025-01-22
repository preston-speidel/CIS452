#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char input[100]; // Buffer to store user input
    const char *exit_command = "exit"; // Command to exit the program
    const char *clear_command = "clear";

    //clear the terminal to give an immersive experience
    system("clear");
    printf("Welcome to the Almighty Shell!\n");
    printf("Type 'exit' to quit.\n");

    while (1) { // Infinite loop to continuously prompt the user
        printf("~ "); // Shell-like prompt
        fgets(input, sizeof(input), stdin); // Get user input (supports spaces)
        
        // Remove trailing newline character from input
        input[strcspn(input, "\n")] = 0;

        //tokenize input
        char *args[2]; //
        char *lineptr = input;               // Pointer to the string
        /*
            Finds the first occurrence of a delimiter in the string.
            Replaces the delimiter with a null terminator (\0).
            Returns the token (a pointer to the start of the string segment).
            Updates the pointer (lineptr) to point to the remaining part of the string.
        */
        args[0] = strsep(&lineptr, " "); //command
        // args[1] = strsep(&lineptr, " "); // flag
        // args[2] = strsep(&lineptr, " "); // flag
        args[1] = NULL;

        // Ensure that we have exactly 3 arguments
        // if (args[0] == NULL || args[1] == NULL || args[2] == NULL) {
        if (args[0] == NULL) {
            printf("Error: Please enter exactly one\n");
            return 1;  // Exit with error code
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
            if (execvp(args[0], args) == -1) {
                perror("execvp failed");
                return 1;
            }
        } else {
            // Wait for the child process to finish
            child = wait(&status);
            // Continue your main program here
            printf("Main program running process\n");
        }
        // Check if the user wants to exit
        if (strcmp(args[0], exit_command) == 0) {
            printf("Exiting shell...\n");
            break;
        }
    }

    return 0;
}
