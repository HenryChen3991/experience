#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main(void)
{
   char str[] = "   Vendor: JetFlash aaa Model: Transcend 32GB   Rev: 1.00";
   char s[20];
   char s1[20];
   char s2[20];
   char s3[20];
   char s4[20];
   char s5[20];
   char *pch = NULL;
   pch = strstr(str,"Vendor");

   sscanf(pch,"Vendor: %[^:]: %[^:]:",s,s1);
   printf("s = %s\n",s);
   printf("s1 = %s\n",s1);
   printf("s2 = %s\n",s2);
   printf("s3 = %s\n",s3);
   
/*
   char token[] = "Vendor:";
   char *buf;
   buf = strtok(str,token);
   printf("buf = %s\n",buf);
*/
   double a = 30867452/1048576.0;
   printf("%f\n",a);
   a = (5.0/2.0);
   printf("%f\n",a);

   int error = -1;
  if(error)
	printf("error : %d\n",error);

   unsigned char s7[10]={0};
   s7[0] = strlen("ab");
   printf("0x%x\n",s7[0]);
   snprintf(&s7[2],strlen("ab")+1,"%s","ab");
   printf("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",s7[0],s7[1],s7[2],s7[3],s7[4],s7[5]);

   char c = -2;
   printf("c = 0x%2x\n",c);

   unsigned short la = 106;
   printf("short la : 0x%04x\n",la);

   unsigned char *lla = (unsigned char*)&la;
   printf("short la upper : 0x%02x , lower : 0x%02x\n",lla[0],lla[1]);


   char *ssptr = "aaa";
   if(strcmp(ssptr,"aaa")==0)
       printf("aa\n");
   else
       printf("bb\n");
}
