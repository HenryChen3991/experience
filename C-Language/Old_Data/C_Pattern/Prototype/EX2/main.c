#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct _person{
	int age;
	char *name;
	struct _person* (*clone)(struct _person* p);
	void (*display)(struct _person* p);
}Person;

#define NAME "Henry"
Person * copy(Person *person){
	Person *p = (Person*)malloc(sizeof(Person));
	p->age = person->age;
	p->name = (char *)malloc(sizeof(NAME));
//	printf("in copy , parm name = %s, len = %d\n",person->name,sizeof(person->name));
	strncpy(p->name,person->name,sizeof(NAME));
	p->clone = person->clone;
	p->display = person->display;
	return p;
}

void display(Person *p){
	printf("name : %10s, addr : 0x%p\n",p->name,p->name);
	printf("age : %10d , addr : 0x%p\n",p->age,&p->age);
	printf("clone method       addr : 0x%p\n",p->clone);
}


int main(void){
	
/*	char * name = NULL;
	
	name = (char *)malloc(sizeof(NAME));
	strncpy(name,NAME,sizeof(NAME));
	printf("name = %s\n",name);
*/

    Person *p = (Person *)malloc(sizeof(Person));
    p->age = 10;
    p->name = (char *)malloc(sizeof(NAME));
    strncpy(p->name,NAME,sizeof(NAME));
    p->clone = copy;
    p->display = display;
    p->display(p);

    Person *p2 = NULL;
    p2 = p->clone(p);
    p2->display(p2);

    printf("Change person p2 age to 15\n");	
    p2->age = 15;
    strncpy(p2->name,"Max\0",4);
    p->display(p);
    p2->display(p2);
    



    return 0;
}
