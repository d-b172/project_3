#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

void mycd(char* argv)
{
 if(chdir(argv) == -1)
  {
    if((S_ISREG(argv) == 0) && (S_ISLNK(argv) == 0))
    {
      perror("-bash: cd: ", argv, ": Not a directory\n");
    } else if(S_ISDIR(argv) == 0)
      {
         perror("-bash: cd: ", argv, ": No such file or directory\n");
      }
  }

}

int main(int argc,char* argv[])
{
  mycd(argv[1]);

  return 0;
}
