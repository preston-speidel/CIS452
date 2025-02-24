#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        printf("Maximum number of open files (soft limit): %llu\n", (unsigned long long)limit.rlim_cur);
    } else {
        perror("getrlimit failed");
        return 1;
    }

    return 0;
}