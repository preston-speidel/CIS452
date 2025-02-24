#include <stdio.h>
#include <unistd.h>

int main() {
    long num_pages = sysconf(_SC_PHYS_PAGES);

    if (num_pages == -1) {
        perror("sysconf failed");
        return 1;
    }

    printf("Total number of physical pages: %ld\n", num_pages);

    return 0;
}