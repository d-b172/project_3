#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "mysh.h"

int main() {
  char *path, *argv[20], buf[80], n, *p;
  int m, status, inword, continu, pipes, rfd[2], lfd[2], *start;
  int infd, outfd;
  char *iofiles[2] = {NULL, NULL};

  while (1) {
    inword = 0;
    p = buf; //p is string input buffer
    m = 0;
    pipes = 0;
    continu = 0;
    iofiles[0] = NULL;
    iofiles[1] = NULL;
    printf("\nshhh> ");
    fflush(stdin);
    while((n = getchar()) != '\n' || continu) {
      if (n == ' ') {
        if (inword) {
          inword = 0;
          *p++ = 0;
        }
      } else if(n == '\n')
        continu = 0;
      else if(n == '\\' && !inword)
        continu = 1;
      else {
        if(!inword) {
          inword = 1;

          argv[m++] = p;

          *p++ = n;
        } else

          *p++ = n;
      }
    }
    if(m == 0) {
      continue;
    }
    *p++ = 0;
    argv[m] = NULL;
    start = (int *)malloc(m*sizeof(int));
    if(strcmp(argv[0], "exit") == 0) exit(0);

    //Count number of pipes in command
    start[0] = 0; // First command starts at index 0
    pipes = 0;
    int i;
    for(i = 0; i < m; i++) {
      if(strcmp(argv[i], "|") == 0) { //Found pipe
        pipes++;
        start[pipes] = i + 1; // Next command starts at index after pipe
        argv[i] = NULL; // Set pipe index to 0 (execvp looks for 0 for end of command)
      }
      else if(strcmp(argv[i], "<") == 0) { //Found input redir
        //BASH does the redirection right away no matter where it sees the redirection 
        //in the command, so typing 1>test.txt ls | sort -r will output the unsorted ls list
        //to the file test.txt and then sort -r will have no input from the pipe and therefore
        //have nothing to sort. For our shell we should do input redirection right away to no
        //matter where we see it in the command, not in the same loop as the piping.
        //We should also take the redirection out of the arguments after processing the redirection
        //and treat the args as if there is not redirection from then on. This is to ensure that the
        //command is executed properly.
        iofiles[0] = argv[i + 1];
        argv[i] = NULL;
      }
      else if(strcmp(argv[i], ">") == 0) { //Found output redir
        iofiles[1] = argv[i + 1];
        argv[i] = NULL;
      }
    }

    //Main loop to handle pipes, redirection
    int j;
    pipe(lfd);
    for(j = 0; j <= pipes; j++) {
      //Create pipe if needed
      if(pipes > 0) {
        pipe(rfd);
      }
      int s_index = start[j];
      pid_t pid = fork();
      if(pid < 0) {
        printf("\nError in forking child. Try again\n");
      }
      else if(pid > 0) { //Parent block
        if(j > 0) {
          close(lfd[0]); //Close unnecessary left pipe
          close(lfd[1]);
        }
        lfd[0] = rfd[0]; // Save current right pipe as left pipe
        lfd[1] = rfd[1];
        wait(&status);
      }
      else if (pid == 0) { //Child block
        if(iofiles[0] != NULL && j == 0) {
          infd = open(iofiles[0], O_RDONLY);
          dup2(infd, 0);
          close(infd);
        }
        if(pipes > 0) {
          if(j == 0) {
            //Handle first pipe
            dup2(rfd[1], 1);
            close(rfd[0]);
            close(rfd[1]);
          }
          else if(j == pipes) {
            //Handle last pipe
            dup2(lfd[0], 0);
            close(lfd[0]);
            close(lfd[1]);
          }
          else {
            //Handle middle pipes
            dup2(lfd[0], 0);
            close(lfd[0]);
            close(lfd[1]);
            dup2(rfd[1], 1);
            close(rfd[0]);
            close(rfd[1]);
          }

        }
        if(iofiles[1] != NULL && j == pipes) {
          //For some reason this won't redirect the command if the redirection comes before the command
          outfd = open(iofiles[1], O_WRONLY|O_CREAT|O_APPEND);
          dup2(outfd, 1);
          close(outfd);
        }
        execvp(*(argv + s_index), argv + s_index);
        printf(" didn't exec \n ");
      }
    }
    free(start);
  }
}