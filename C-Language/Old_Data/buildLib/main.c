#include <stdio.h>
#include "sum.h"
int main(int argc, char *argv[]) {
  double a = 2.6, b = 4.2, c;
  extern int val;
  printf("val = %d\n",val);
  val = 4;
  printf("val = %d\n",val);
  c = sum(a, b);
  c = sum(a, b);
  printf("%.1f + %.1f = %.1f\n", a, b, c);
  return 0;
}
