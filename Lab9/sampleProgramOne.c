#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
    ssize_t bufsize = SIZE;
    char *data1;

    data1 = malloc (SIZE);
    printf ("Please input username: ");
    ssize_t nread = getline(&data1, &bufsize, stdin);
    if (nread == -1) {
        perror("getline");
        free(data1);
        return 1;
    }
    // Remove the newline character, if present
    if (nread > 0 && data1[nread - 1] == '\n') {
        data1[nread - 1] = '\0';
    }
    printf ("You entered: [%s]\n", data1);
    free (data1);
    return 0;
}
