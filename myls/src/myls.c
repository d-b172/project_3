#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_SIZE 256

int L_FLAG = 0;
int ARG_FLAG = 0;
char *in_args[MAX_SIZE];

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
        in_args[j] = NULL;
        int i;
        for(i = 0; i < j; i++) {
            printf("%s ", in_args[i]);
        }
        printf("\n");
    }
    
}