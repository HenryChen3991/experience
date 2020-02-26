#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct kool_list{
    int to;
    struct list_head list;
    int from;
};

int main(int argc, char **argv){

    struct kool_list *tmp;
    struct list_head *pos, *q;
    unsigned int i;

    struct kool_list mylist;
    INIT_LIST_HEAD(&mylist.list);

    for(i=5; i!=0; --i){
        tmp= (struct kool_list *)malloc(sizeof(struct kool_list));
        printf("enter to and from:");
        scanf("%d %d", &tmp->to, &tmp->from);
        list_add(&(tmp->list), &(mylist.list));
    }
    printf("\n");

    printf("traversing the list using list_for_each_entry()\n");
    list_for_each_entry(tmp, &mylist.list, list)
        printf("to= %d from= %d\n", tmp->to, tmp->from);
    printf("\n");

    printf("deleting the list using list_for_each_safe()\n");
    list_for_each_safe(pos, q, &mylist.list){
        tmp= list_entry(pos, struct kool_list, list);

    printf("freeing item to= %d from= %d\n", tmp->to, tmp->from);
        list_del(pos);
        free(tmp);
    }
    return 0;
}
