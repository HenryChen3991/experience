#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 

// 定义Clonable接口 
typedef void* (*Clone)(); 
typedef struct { 
    Clone copy; 
}Clonable; 

// Person类扩展Clonable接口 
typedef struct { 
    Clonable base; 
    unsigned int age; 
    char *name;
}Person; 
#define NAME "BBBBBB"

Clonable* ClonePerson(Person *person) 
{ 
    printf("ClonePerson\n"); 
    Person *p = (Person *)malloc(sizeof(Person)); 
    memset(p, 0, sizeof(Person)); 
    p->age = person->age; 
    p->name = (char *)malloc(sizeof(strlen(NAME)));
    strncpy(p->name,person->name,sizeof(NAME));
    return (Clonable *)p; 
}; 

// 创建Clonable对象的Factory 
Clonable* ClonableFactory(Clonable* clonable) 
{ 
 //   printf("%d\n", ((Person *)clonable)->age); 
    if(0 == clonable) 
        return 0; 
    return clonable->copy(clonable); 
} 

// 用法 
int main() { 
    Person p; 
    p.base.copy = ClonePerson; 
    p.age = 4;
    p.name = (char *)malloc(sizeof(strlen(NAME)));
    strncpy(p.name,NAME,sizeof(p.name));
    printf("p name = %s\n",p.name);
    Person* anotherPerson = NULL; 
    anotherPerson = (Person *)ClonableFactory(&p); 
    printf("anotherPersion age %d,name %s\n", anotherPerson->age,anotherPerson->name); 
}
