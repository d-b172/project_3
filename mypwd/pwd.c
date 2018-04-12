#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

void mypwd()
{
  char path[PATH_MAX];
  if(getcwd(path, sizeof(path)) == NULL)
    perror("getcwd() error");
  else
    printf("%s\n", path);
}


int main()
{
  mypwd();  

  return 0;        
}
