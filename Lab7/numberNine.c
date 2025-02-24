#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        printf("Maximum number of open files (hard limit): %llu\n", (unsigned long long)limit.rlim_max);
    } else {
        perror("getrlimit failed");
        return 1;
    }

    return 0;
}