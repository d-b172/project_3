#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX_SIZE 256

int L_FLAG = 0;
int ARG_FLAG = 0;
char *in_args[MAX_SIZE];
struct stat buf;
char permissions[11];
char timebuf[50];
int numLinks;
int size;

int main(int argc, char **argv) {
    int opt, argsind = 1;
    while((opt = getopt(argc, argv, "l")) != -1) {
        switch(opt) {
            case 'l':
                L_FLAG = 1;
                break;
            default:
                break;
        }
    }
    argsind = optind;

    if(argsind != argc) { //If args
        ARG_FLAG = 1;
        int i, j = 0;
        //Get arguments copied into another array
        for(i = argsind; i < argc; i++) {
            in_args[j] = malloc((strlen(argv[i]) + 1) *sizeof(char));
            strncpy(in_args[j], argv[i], strlen(argv[i]) + 1);
            j++;
        }
        in_args[j] = NULL;
        /* If we pass myls a directory as an argument, we can open the directory
           and get all the files in it and print ls infor about them, do this for all
           directories passed */  
    }
    else if(argsind == argc) { //If no args
        //Do command for all files in directory
        DIR *dir = opendir(".");
        struct dirent *dp;
        char *file_name;
        int j = 0;
        while((dp = readdir(dir)) != NULL) {
            file_name = dp->d_name;
            if(strcmp(file_name, ".") != 0 && strcmp(file_name, "..") != 0) { //Demorgan's law strikes again
                in_args[j] = malloc((strlen(file_name) + 1) * sizeof(char));
                strncpy(in_args[j], file_name, strlen(file_name) + 1);
                j++;
            } 
        }
        int i;
        in_args[j] = NULL;
        if(L_FLAG){
            for(i = 0; i < j; i++){
                stat(in_args[i],&buf);
                permissions[0] = (S_ISDIR(buf.st_mode) ? 'd' : '-');
                permissions[1] = (buf.st_mode & S_IRUSR) ? 'r' : '-';
                permissions[2] = (buf.st_mode & S_IWUSR) ? 'w' : '-';
                permissions[3] = (buf.st_mode & S_IXUSR) ? 'x' : '-';
                permissions[4] = (buf.st_mode & S_IRGRP) ? 'r' : '-';
                permissions[5] = (buf.st_mode & S_IWUSR) ? 'w' : '-';
                permissions[6] = (buf.st_mode & S_IXUSR) ? 'x' : '-';
                permissions[7] = (buf.st_mode & S_IROTH) ? 'r' : '-';
                permissions[8] = (buf.st_mode & S_IWOTH) ? 'w' : '-';
                permissions[9] = (buf.st_mode & S_IXOTH) ? 'x' : '-';
                permissions[10] = '\0';
                numLinks = buf.st_nlink;
                strncpy(timebuf, ctime(&buf.st_atime), 49);
                timebuf[49] = '\0';
                int len = strlen(timebuf);
                if(timebuf[len-1]=='\n') timebuf[len-1] = '\0';
                int size = buf.st_size;
                struct passwd *user = getpwuid(buf.st_uid);
                struct group *grp = getgrgid(buf.st_gid);
                char u[50];
                strncpy(u, user->pw_name, 49);
                u[49] = '\0';
                char g[50];
                strncpy(g, grp->gr_name, 49);
                g[49] = '\0';
                printf("%s %d %s %s %d %s %s\n", permissions, numLinks, u, g, size, time, in_args[i]);
            }
        }
        else{
            for(i = 0; i < j; i++) {
                printf("%s ", in_args[i]);
            }
            printf("\n");
        }
    }
    
}