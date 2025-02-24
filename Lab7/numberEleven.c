#include <stdio.h>
#include <time.h>

int main() {
    struct timespec res;

    if (clock_getres(CLOCK_REALTIME, &res) == 0) {
        double resolution_ms = (res.tv_sec * 1000.0) + (res.tv_nsec / 1000000.0);
        printf("Clock resolution: %.6f milliseconds\n", resolution_ms);
    } else {
        perror("clock_getres failed");
        return 1;
    }

    return 0;
}