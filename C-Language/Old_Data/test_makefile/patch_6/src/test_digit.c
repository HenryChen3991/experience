#include <stdio.h>
#include <stdlib.h>

void main(void){
   int d1 = 4;
   int d2 = 5;
   unsigned char str;
   str = (d1 << 4 ) | d2;
   printf("%c\n",str);

}
