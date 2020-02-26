#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stu{
	char name[10];
	int age;
	char addr[10];
}Stu_t;

void show(char *str){
	printf("%s\n",str);
}

void main(){
    char str[256]; 
    Stu_t s = { "Henry",11,"aaa" };
    printf("%s\n",s.name);
     
    memcpy(str,&s,sizeof(str));
    printf("%s\n",str);


    Stu_t a;
    memcpy(&a,str,sizeof(a));
    printf("%s\n",a.addr);


    char aa[128] = "henry";
    show(aa);



    int ret = 99;
    int a1 = 5;
    ret = (a1 != 5);
    printf("ret = %d\n",ret);
}





