#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main(void)
{
   char *str="Jan  1 00:04:31 (none) syslog.info syslogd started: BusyBox v1.27.2  73\n";
   printf("len = %d",strlen(str));

   printf("str[71] = %d",str[71]);
char *str1="Jan  1 00:04:31 (none) syslog.info syslogd started: BusyBox v1.27.2";
printf("len = %d",strlen(str1));

char *str2 ="Jan  1 00:44:34 (none) daemon.warn radvd[1608]: received RA from fe80::290:4bff:fe12:3456  95\n";
printf("len = %d",strlen(str2));


   char buffer[1024] = "hello world";
  printf("buffer = %s\n",buffer);
  char *str3 = NULL;
  str3 = buffer;
  printf("str3 = %s\n",str3);
  printf("str3 len = %d\n",strlen(str3));
  str3 += 5;
  printf("str3 = %s\n",str3);
}

