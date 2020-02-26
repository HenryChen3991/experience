#include <stdio.h>
#include <stdlib.h>
//#include <linux/list.h>
struct num {
     struct list_head node;
     int number;
};

void main()
{
     LIST_HEAD(head);
     struct num *tmp;
     struct list_head *iterator;
     int i;   

     for (i=0 ; i<5 ; i++) {
         tmp = malloc(sizeof(struct num));
         tmp->number = i;
         list_add(&tmp->node, &head);
     }

     list_for_each(iterator, &head) {
         printf("%dn", list_entry(iterator, struct num, node)->number);
     }
}
