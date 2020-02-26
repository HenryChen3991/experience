#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
 int age;
 int mon;
}Node_t;

typedef struct Stu{
 char name[20];
 Node_t node;
}Stu_t;

typedef struct Class{
   int cls_num;
   Stu_t stu[2];
}Class_t;


int main ()
{
    char str[] = "memmove can be very useful......";
    memmove (str,str+15,11);
    puts (str);
    Class_t cls;
    Class_t *cls_p = &cls;
    cls.stu[0].node.age = 1;
    cls.stu[0].node.mon = 2;
    strncpy(cls.stu[0].name,"ja",sizeof(cls.stu[0].name));
    cls.stu[1].node.age = 3;
    cls.stu[1].node.mon = 4;
    strncpy(cls.stu[1].name,"Henry",5);
    cls.cls_num = 2;
    printf("name = %s , age = %d , mon = %d\n",cls.stu[0].name,cls.stu[0].node.age,cls.stu[0].node.mon);
    printf("name = %s , age = %d , mon = %d\n",cls.stu[1].name,cls.stu[1].node.age,cls.stu[1].node.mon);
    int i=0,j=1;
    memmove(&cls_p->stu[i],&cls_p->stu[j],sizeof(Stu_t));
    printf("name = %s , age = %d , mon = %d\n",cls.stu[0].name,cls.stu[0].node.age,cls.stu[0].node.mon);
    printf("name = %s , age = %d , mon = %d\n",cls.stu[1].name,cls.stu[1].node.age,cls.stu[1].node.mon);

    memset(&cls_p->stu[j],0,sizeof(cls_p->stu[j]));
    printf("name = %s , age = %d , mon = %d\n",cls.stu[0].name,cls.stu[0].node.age,cls.stu[0].node.mon);
    printf("name = %s , age = %d , mon = %d\n",cls.stu[1].name,cls.stu[1].node.age,cls.stu[1].node.mon);

    return 0;
}
