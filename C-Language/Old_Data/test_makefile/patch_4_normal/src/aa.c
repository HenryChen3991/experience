#include <stdio.h>
#include <stdlib.h>

void aa(int s,char * str)
{
 printf("s = %d\n",s);
 s = 1;
 printf("s = %d\n",s);

 if(!str){
  printf("str is  null\n");
 }else{
  printf("str is not null\n");
  printf("str = %s\n",str);
 }
}

void main(void)
{
 int a = 3;
 char * str = "aaaa";
 aa(a,str);
 printf("a = %d\n",a);
}
