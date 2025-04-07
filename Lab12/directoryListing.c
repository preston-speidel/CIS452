#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    //flags for options
    int showIDs = false;
    int showInode = false;
    if (argc >= 3) {
        if (strchr(argv[2], 'n')) {
            showIDs = true;
        }
        if (strchr(argv[2], 'i')) {
            showInode = true;
        }
    }
    
    DIR *dirPtr = opendir(argv[1]);

    struct dirent *entry;
    struct stat statBuf;
    char path[1024];
    
    //print column headers
    printf("%-30s", "Filename");
    if (showInode) {
        printf("%-15s", "Inode");
    }
    if (showIDs) {
        printf("%-8s %-8s", "UID", "GID");
    }
    printf("\n");
    printf("--------------------------------------------------------------\n");
    
    while ((entry = readdir(dirPtr))) {
        //build full path for the entry
        snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);
        
        stat(path, &statBuf);
        
        //file name with fixed column width
        printf("%-30s", entry->d_name);
        
        //inode number
        if (showInode) {
            printf("%-15llu", (unsigned long long)statBuf.st_ino);
        }
        //user and group IDs
        if (showIDs) {
            printf("%-8d %-8d", statBuf.st_uid, statBuf.st_gid);
        }
        printf("\n");
    }
    
    closedir(dirPtr);
    return 0;
}