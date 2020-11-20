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
}
