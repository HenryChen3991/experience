#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LIST_ENTRY(type) \
struct { \
   struct type *next; \
}

#define LIST_HEAD(name,type) \
struct name { \
    struct type *first; \
    struct type *last; \
}

#define LIST_HEAD_INIT(head){     \
	(head)->first = NULL;     \
	(head)->last = NULL;	  \
//	mutexInit(&(head)->lock); \
}


#define LIST_FIRST(head) \
	((head)->first)

#define LIST_INSERT_TAIL(head, element, field) do {         \
	if(!(head)->first){                                 \
		(head)->first = (element);                  \
        	(head)->last = (element);                   \
	}                                                   \
	else{                                               \
        	(head)->last->field.next = (element);       \
        	(head)->last = (element);		    \
	} 						    \
}while(0)

#define LIST_TRAVERSE(head,var,field)	\
	for( (var)= (head)->first ; (var) ; (var) = (var)->field.next )

typedef void (*NodeOperate)(void);
typedef struct Node{
    int age;
    char name[32];
    NodeOperate opt;
    LIST_ENTRY(Node) list;
}Node_t;
LIST_HEAD(StudentList,Node);
static struct StudentList stuList;

void showAge(struct Node *item){
   printf("node age = %d\n",item->age);
}

void doSomething_Henry(void){
	printf("request Henry to do homework\n");
}


int main(void)
{
   Node_t *item;
   item = (struct Node*)malloc(sizeof(struct Node));
   if( item == NULL ){
	printf("can't malloc\n");
	return -1;
   }

   item->age = 10;
   strncpy(item->name,"Henry",sizeof(item->name));
   item->opt = doSomething_Henry;

   LIST_INSERT_TAIL(&stuList,item,list);

   item = NULL;
   
   item = (struct Node*)malloc(sizeof(struct Node));
   if( item == NULL ){
        printf("can't malloc\n");
        return -1;
   }

   item->age = 15;
   strncpy(item->name,"Jim",sizeof(item->name));

   LIST_INSERT_TAIL(&stuList,item,list);
   
//Using function pointer example.
//   stuList.first->opt();

   item = NULL;
   LIST_TRAVERSE(&stuList,item,list){
	printf("age = %d\n",item->age);
	if( item->opt != NULL )
		item->opt();
   }
}
