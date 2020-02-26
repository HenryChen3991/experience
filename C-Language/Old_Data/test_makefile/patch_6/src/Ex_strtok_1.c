#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
   char str[250] = "Content-Disposition: form-data; name=\"filename\"; filename=\"Desert.jpg\"";
   char *delim = "=";
   char *pch;
/*
   printf("str = %s\n",str);
   pch = strtok(str,delim);
   while(pch != NULL){
	printf("%s\n",pch);
	pch = strtok(NULL,delim);
   } 
*/

   //printf("use strstr\n");
   pch = strstr(str,"filename=");
   printf("pch = %s\n",pch);
   printf("%s\n",&pch[9]);
   
   char *str2 = "\"Desert.jpg\"\n";
   char file[50]={0};

   strncpy(file,str2+1,strlen(str2)-3);


   printf("%s\n",file);

}


