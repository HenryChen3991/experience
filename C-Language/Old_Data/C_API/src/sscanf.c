#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
   int day, year;
   char weekday[20], month[20], dtm[100];
   char name[20];
   strcpy( dtm, "Saturday March 25 1989" );
   sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );

   printf("%s %d, %d = %s\n", month, day, year, weekday );


   sscanf("name:john age:40 tel:082-313530", "%s", name);
   printf("%s\n", name);


   unsigned char mac[6] = {0};
   int i = 0;
   for(i=0;i<6;i++)
       printf("0x%x\n",mac[i]);

//   sscanf("11:22:33:44:55:66","%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
   if(argv[1]!=NULL)
       sscanf(argv[1],"%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
   for(i=0;i<6;i++){
       printf("0x%x\n",mac[i]);
       unsigned char c = mac[i];
       printf("c =  %c 0x%x\n",c,c);

   }
   return(0);
}
