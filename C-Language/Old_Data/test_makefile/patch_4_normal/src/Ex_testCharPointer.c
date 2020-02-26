#include <stdio.h>
#include <stdlib.h>
#include "def.h"
char image[128]={0};
char * getI(void){
	return image;
}
void main(void)
{
  strncpy(image,"ssssss",127);
  printf("%s\n",image);
  printf("%s\n",getI());
  char *c1="A";
  printf("%c\n",*c1);
}
