#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
        printf("Maximum file size in bytes:\n");
        printf("Soft limit: %llu bytes\n", (unsigned long long)limit.rlim_cur);
        printf("Hard limit: %llu bytes\n", (unsigned long long)limit.rlim_max);
    } else {
        perror("getrlimit failed");
        return 1;
    }

    return 0;
}