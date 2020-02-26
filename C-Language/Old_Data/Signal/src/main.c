#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void usage(void);
#define case 9
int main(int argc,char *argv[])
{
   char cmd[50]={0};
   printf("Test linux signal program\n");
   if(argc==2 || argc==3)
   {
       if(argc==2){
           if(atoi(argv[1])>=1 && atoi(argv[1])<=case ){
		snprintf(cmd,sizeof(cmd),"./bin/EX%s_signal",argv[1]);
		system(cmd);
           }
	   else
	   {
		usage();
	   }
       }
       else if(argc==3){
	   if( (atoi(argv[1])>=1 && atoi(argv[1])<=9) && !strcmp(argv[2],"-c") ){
		snprintf(cmd,sizeof(cmd),"cat ./src/EX%s_signal.c",argv[1]);
		system(cmd);
	   }
       }
   }
   else
   {
       usage();
   } 
//   system("kill -l");
//   system("./bin/EX1_signal");

   return 0;
}
void usage(void)
{
	printf("EX1~EX7 are about signal() example\n");
	printf("EX8~EX10 are about sigaction() example\n");
        printf("./main 1~%d (-c)\n",case);
}
