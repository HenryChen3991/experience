#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	FILE *fp;
   char str[60];
	char path[]="./aa/b.txt";
   /* opening file for reading */
   fp = fopen(path , "r");
   if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
   while( fgets (str, 60, fp)!=NULL ) {
      /* writing content to stdout */
      printf("%s",str);
   }
   
	return 0;
}

