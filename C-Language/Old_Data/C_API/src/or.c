#include <stdio.h>
#include <stdlib.h>

int main()
{
#define a 0xc000
#define b 0x2000
#define ans (a|b|1)

   printf("0x%x\n",ans);
   printf("%d\n",ans);
   
   int n = 2;

   n = --n < 0 ? 0 : n;

   printf("n = %d\n",n);
   return(0);
}
