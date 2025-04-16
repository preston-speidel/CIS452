// where did this come from?<stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define SIZE 30

int main(int argc, char *argv[])
{
    struct flock fileLock;
    int fd;
    char myBuffer[SIZE] = "// where did this come from?";

    if (argc < 2) {
        printf ("usage: filename\n");
        exit (1);
    }
    if ((fd = open (argv[1], O_RDWR)) < 0) {
        perror ("there is");
        exit (1);
    }

    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    if (fcntl (fd, F_SETLKW, &fileLock) < 0) {
        perror ("Unable to set file lock");
        exit (1);
    }

    write (fd, myBuffer, SIZE-2);

    sleep (10);

    //release the file lock
    fileLock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fileLock) < 0) {
        perror("Unable to release file lock");
        exit(1);
    }

    close(fd);
    
    return 0;
}