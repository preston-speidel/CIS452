#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NPROC, &limit) == 0) {
        printf("Maximum number of processes per user:\n");
        printf("Soft limit: %llu\n", (unsigned long long)limit.rlim_cur);
        printf("Hard limit: %llu\n", (unsigned long long)limit.rlim_max);
    } else {
        perror("getrlimit failed");
        return 1;
    }

    return 0;
}