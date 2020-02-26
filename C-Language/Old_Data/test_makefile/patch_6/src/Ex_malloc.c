#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student{
    char name[30];
    int age;
}stu;

void main(void)
{
  stu *stu1 = NULL;
  stu1 = malloc(0);
  
  strncpy(stu1->name,"Henrygggggggggggggggggggggggggggggggggggggggggggggggggggggggaaaaaaaaaaaaaaaaaaaaaaaaaahhhhhhhhhhhhhhhhhhhhhhhhhhh",sizeof(stu1->name));
  stu1->age = 10;
  
  char str1[100]={0};

  strncpy(str1,"aaaaaaaaaaaaaaakkkkkkkkkkkkkkkkkkkkkkk",sizeof(str1));
  printf("name = %s\n",stu1->name);
  printf("age = %d\n",stu1->age);
  printf("str1 = %s\n",str1);
}
