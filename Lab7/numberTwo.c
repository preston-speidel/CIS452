#include <stdio.h>
#include <limits.h>
#include <semaphore.h>

int main() {
    // Static lookup for maximum semaphore value
    #ifdef SEM_VALUE_MAX
        printf("Maximum value of a counting semaphore (static): %d\n", SEM_VALUE_MAX);
    #else
        printf("SEM_VALUE_MAX is not defined in limits.h\n");
    #endif

    return 0;
}