#include "mysh.h"

struct command commands[MAX_SIZE];
char *cwd;
char PROMPT[MAX_SIZE];
int num_commands;

void setup_shell() {
    //do some stuff to set up the shell
    //allocate cwd
    cwd = malloc(sizeof(char)*PATH_MAX);
    if(getcwd(cwd, PATH_MAX) == NULL) {
        perror("Error with getcwd");
    }  
    strcpy(PROMPT, "mysh:");
    strcat(PROMPT, cwd);
    strncat(PROMPT, "> ", 2);
}



int main(int argc, char **argv) {
    char in_line[MAX_LN_SZ];
    setup_shell();
    while(1) {
        //Main loop
        printf("%s", PROMPT);
        in_line[0] = 0;
        fgets(in_line, MAX_LN_SZ, stdin); //get line from input
        //Check to ensure line isn't blank and doesn't lead with spaces
        if(in_line[0] == ' ') {
            continue;
        }
        if(strcmp(in_line, "exit") == 0) {
            break;
        }
    }
    free(cwd);
    exit(0);
}