//assistance from chatgpt
#include <stdio.h>
#include <stdlib.h>

// Initialized global variable
int global_init = 100;

// Uninitialized global variable
int global_uninit;

// sampleFunction is part of the Text segment (code segment).
void sampleFunction() {
    // Print the address of sampleFunction.
    printf("Address of sampleFunction (Text segment): %p\n", (void *)sampleFunction);
}

/*
 * Recursive function to demonstrate stack growth.
 * It prints the address of a local variable at each recursion level.
 * Comparing the addresses between recursive calls shows the growth direction.
 */
void recursiveStack(int depth, void *prev_address) {
    // Local variable resides on the stack.
    int local;  
    printf("Depth %d, address of local variable: %p", depth, (void *)&local);
    
    // If we have a previous address, compare it with the current one.
    if (prev_address != NULL) {
        if (&local < prev_address) {
            printf("  --> Stack grows downward.\n");
        } else {
            printf("  --> Stack grows upward.\n");
        }
    } else {
        printf("\n");
    }
    
    // Limits the recursion
    if (depth < 5) {
        recursiveStack(depth + 1, (void *)&local);
    }
}

int main(void) {
    // Print the address of main (Text segment)
    printf("Address of main (Text segment): %p\n", (void *)main);
    
    // Call sampleFunction to print its address (Text segment)
    sampleFunction();
    
    // Print addresses of global variables
    printf("Address of global_init (Initialized Data): %p\n", (void *)&global_init);
    printf("Address of global_uninit (Uninitalized BSS): %p\n", (void *)&global_uninit);
    
    // Heap segment:
    // Allocates two small blocks on the heap
    int *heap1 = malloc(sizeof(int));
    int *heap2 = malloc(sizeof(int));
    if (heap1 == NULL || heap2 == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    printf("Address of first heap allocation (Heap): %p\n", (void *)heap1);
    printf("Address of second heap allocation (Heap): %p\n", (void *)heap2);
    
    // Determines the growth direction for the heap
    if (heap2 > heap1) {
        printf("Heap appears to grow upward (increasing addresses).\n");
    } else {
        printf("Heap appears to grow downward (decreasing addresses).\n");
    }
    
    // Stack segment
    // Local variable in main shows an address on the stack
    int local_var = 0;
    printf("Address of a local variable in main (Stack): %p\n", (void *)&local_var);
    
    // recursive function to illustrate how the stack grows
    printf("\nDemonstrating stack growth with recursive function:\n");
    recursiveStack(1, NULL);
    
    // Free allocated heap memory.
    free(heap1);
    free(heap2);
    
    return 0;
}
