#include <stdio.h>
#include <string.h>

void main(void){
  char s[] = "The spirit is willing but the flesh is weak.";
  char t[] = "but";
  printf("%d\n",strcspn(s,t));
}
