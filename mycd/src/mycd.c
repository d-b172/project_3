void mycd(char* argv)
{
 struct stat buff;
 stat(argv, &buff);
 if(chdir(argv) == -1)
  {
    if((S_ISREG(buff.st_mode) != 0) || (S_ISLNK(buff.st_mode) != 0))
    {
      printf("-bash: cd: ");
      printf("%s", argv);
      printf(": Not a directory\n");
    } else if(S_ISDIR(buff.st_mode) == 0)
      {
         printf("-bash: cd: ");
         printf("%s", argv);
         printf(": No such file or directory\n");
      }
  }

}
