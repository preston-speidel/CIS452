#include <stdio.h>
#include <limits.h> 
#include <unistd.h>

int main() {
    #ifdef _SC_SEM_NSEMS_MAX
        long max_semaphores = sysconf(_SC_SEM_NSEMS_MAX);
        if (max_semaphores != -1)
            printf("Maximum number of semaphores per process: %ld\n", max_semaphores);
        else
            printf("Could not determine the maximum number of semaphores.\n");
    #endif

    return 0;
}