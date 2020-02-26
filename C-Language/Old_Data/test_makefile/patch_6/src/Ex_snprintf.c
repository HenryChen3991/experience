#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
   char cmdline[128]={0};
   char str[10] = "Test1";
   char str2[10] = "Test2";
   int len = 0;

   len = snprintf(cmdline,sizeof(cmdline),"cmd=%s",str);

   printf("%s\n",cmdline);
   printf("len = %d\n",len);

   len = snprintf(&cmdline[len],sizeof(cmdline)," -b %s",str2);
   
   printf("%s\n",cmdline);
   printf("len = %d\n",len);

}
