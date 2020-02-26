#include <stdio.h>
#include <stdlib.h>

void main(int argc,char *argv[])
{
   char *s[] = { "main","git","java","python"};
   char **ptr=  NULL;

   ptr = &s[0];
   printf("ptr = %s\n",*ptr);
#ifdef MFlag
   printf("MFlag = 1\n");
#else
   printf("MFlag = 0\n");
#endif //MFlag
}
