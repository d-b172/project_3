#include "mysh.h"

void mypwd()
{
  char path[PATH_MAX];
  if(getcwd(path, sizeof(path)) == NULL)
    perror("getcwd() error");
  else
    printf("%s\n", path);
}

