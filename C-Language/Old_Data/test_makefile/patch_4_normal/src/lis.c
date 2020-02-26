#include <stdio.h>      /* printf */
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

void PrintFloats (const char *n, ...)
{
  int i;
  double val;
  //printf ("Printing floats:");
  va_list vl;
  va_start(vl,n);
  while(n){
	printf("%s\n",n);
	n = va_arg(vl,const char *);
  }
  va_end(vl);
  //printf ("\n");
}

int main ()
{
  PrintFloats ("3","3.14159","2.71828","1.41421",NULL);
  PrintFloats ("3","3.14159","2.71828","1.41421",NULL);
  return 0;
}
