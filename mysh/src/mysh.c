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

void tokenize_commands(char *command_string) {
    char *tok, *toktok, *str1, *str2, *p1, *p2;
    int i = 0, j = 0, k = 0, c;
    int string_len;

    char copy_str[MAX_LN_SZ];
    /* The string passed might have the format >file or <file for redirection,
       and we want a space between those tokens, so we're going to loop through
       the array and build a new array with spaces if they aren't there. */
    int ch_flag = 0; //Flag if something was changed
    for(i = 0; command_string[i] != '\0'; i++) {
        ch_flag = 0;
        c = command_string[i];
        if(c == '<') {
            copy_str[k++] = ' ';
            copy_str[k++] = '<';
            copy_str[k++] = ' ';
            ch_flag = 1;
        }
        else if(c == '>') {
            copy_str[k++] = ' ';
            copy_str[k++] = '>';
            copy_str[k++] = ' ';
            ch_flag = 1;
        }
        if(ch_flag == 0) {
            copy_str[k++] = c;
        }
    }
    copy_str[k++] = '\0';
    /* DEBUG: Write original command_string array and new copy_string to stdout */
    // printf("%s\n", command_string);
    // printf("%s\n", copy_str);

    strncpy(command_string, copy_str, strlen(copy_str));

    /* The string that we have now is only a string, so we want to iterate
       through it and tokenize it into the array of command structs including
       a flag whether the command needs redirection or not and the file name
       if it does need redirection. We use pipes to separate simple commands
       and spaces to separate command tokens. */
    //This for loop initializes str1 to command_string and then for each subsequent iteration of the loop
    //it will set str1 to NULL. This is for strtok which expects a string at first
    //and then NULL for every subsequent call
    for(str1 = command_string; ; str1 = NULL) {
        k = 0;
        if((tok = strtok_r(str1, "|", &p1)) == NULL) {
            break;
        }
        for(str2 = tok; ; str2 = NULL) {
            if((toktok = strtok_r(str2, " ", &p2)) != NULL) {
                if(strcmp(toktok, "<") == 0) { //Found input redirection in curr command
                    commands[j].redirIn = 1; //Set input redirection flag to true
                    toktok = strtok_r(str2, " ", &p2); //Next token should be infile
                    strncpy(commands[j].inFile, toktok, strlen(toktok));
                }
                else if(strcmp(toktok, ">") == 0) {
                    commands[j].redirOut = 1;
                    toktok = strtok_r(str2, " ", &p2);
                    strncpy(commands[j].outFile, toktok, strlen(toktok));
                }
                else {
                    commands[j].tokens[k++] = toktok;
                }
            }
            else { break; }
        }
        commands[j].tokens[k] = NULL;
        commands[j].argc = k;
        j++;
        
    }
    num_commands = j; //Set the global variable num_commands to the number of commands parsed
}


int main(int argc, char **argv) {
    char in_line[MAX_LN_SZ];
    setup_shell();
    while(1) {
        //Main loop
        printf("%s", PROMPT);
        in_line[0] = 0;
        fgets(in_line, MAX_LN_SZ, stdin); //get line from input
        in_line[strlen(in_line) - 1] = '\0'; //Get rid of trailing newline
        //Check to ensure line isn't blank and doesn't lead with spaces
        if(in_line[0] == ' ') {
            continue;
        }
        if(strcmp(in_line, "exit") == 0) {
            break;
        }
        tokenize_commands(in_line);
        printf("Parsed %d simple commands.\n", num_commands);
    }
    free(cwd);
    exit(0);
}