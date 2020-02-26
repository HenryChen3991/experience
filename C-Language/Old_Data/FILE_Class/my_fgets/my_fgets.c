#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FF "1"
char bf[64] = {0};

int main()
{
   FILE *fp;
   unsigned char str;
   printf("bf = %d\n",atoi(bf));
   if(atoi(bf)==0)
	printf("bf == 0\n");

   //using access() to detect file whether exist
   if( access("Makefile",0) == 0)
	printf("file exist\n");
   else
	printf("file not exist\n");


   /* opening file for reading */
   fp = fopen("Makefile" , "r");
//   fp = popen("Makefile" , "r");
   if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
   else{
     printf("fp != NULL\n");
   }
//   while( fgets (str, 60, fp)!=NULL ) {
      /* writing content to stdout */
//      printf("%s",str);
//   }

   unsigned char r = 0xff;
   	if( (str=fgetc(fp))!=EOF){
       	// r = str;
		r = 0x30;
        	printf("str = %x ,%c\n",str,str);
		printf("r = %x %c\n",r,r);
        	if( str == 0xff)
			printf("str = 0xff \n");
        	if( r == str)
 			printf("equal\n");
		else
			printf("not equal\n");	
   	}else{
		printf("get EOF\n");
   	}
   fclose(fp);
   char buffer[256];
   fp = fopen("number","r");
   if(fp==NULL)

	return (-1);

   if(fgets (buffer, 256, fp)!=NULL){
	printf("buffer to int = %d\n",atoi(buffer));
   }

   fclose(fp);
   
   fp = popen("cat log | grep Revision | sed \'s/Revision: //g\'","r");
   if(fp==NULL)
	return (-1);
   if(fgets (buffer, 256, fp)!=NULL){
	printf("buffer = %s",buffer);
        printf("buffer to int = %d\n",atoi(buffer));
   }



   return(0);
}
