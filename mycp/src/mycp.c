#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }
    struct stat s_stat; //Stat struct for source
    struct stat d_stat; //Stat struct for desitnation

    //TODO: Add -r/-R option for recursive copying
    if(stat(argv[1], &s_stat) == -1) {
        perror("Can't stat source.\n");
        exit(1);
    }
    if(stat(argv[2], &d_stat) == -1) {
        perror("Can't stat destination.\n");
        exit(1);
    }
    
    mode_t s_mode = (s_stat.st_mode & S_IFMT);
    mode_t d_mode = (d_stat.st_mode & S_IFMT);

    //Test source to determine if file or directory
    if(s_mode == S_IFDIR) {
        printf("Source is a directory.\n");
    }
    else if(s_mode == S_IFREG) {
        printf("Source is a regular file.\n");
    }
    else {
        perror("Source isn't a directory or regular file.\n");
    }
    
    //Test destination
    if(d_mode == S_IFDIR) {
        printf("Dest is a directory.\n");
    }
    else if(d_mode == S_IFREG) {
        printf("Dest is a regular file.\n");
    }
    else {
        perror("Dest isn't a directory or a regular file.\n");
    }

    exit(0);
}