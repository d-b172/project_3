#include<stdio.h>
#include<string.h>
#define MAX_FILE_NAME_CHARS 128
int main(int argc, char *argv[])
{
 FILE *fp;
 char file_name[MAX_FILE_NAME_CHARS],
      ch;       
 int i;
 
 //if there is no arguments for mycat command
 if(argc < 2)
 {
    printf("** mycat <filename> **\n");
    return 1;
 }

 printf("\n");

 //for-loop for iterating through i amount of
 //files since cat can handle more than one file
 for(i = 1; i < argc; i++)
 {
    //for each file, strncpy to file_name
    strncpy(file_name, argv[i], MAX_FILE_NAME_CHARS);
   
    //open file
    fp = fopen(file_name, "r");
    if(fp == NULL) 
    {
       printf("%s: No such file or directory\n", file_name);
       return 0;
    }

    //read char by char and display to output
    while((ch = fgetc(fp)) != EOF){
         printf("%c",ch);
    }

    printf("\n");
  
    printf("HERERERERERERERE\n");
    //close file
    fclose(fp); 
 }
 printf("NOOO\n");

return 0;
}
