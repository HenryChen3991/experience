#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int j,k,h,t;

    QUEUE* myQ;
    myQ=Queue_Create();
    k=1000;

    for (j = 0; j<QUEUE_SIZE; j++)
    {
        h=myQ->head;
        myQ->insert(myQ, k);
        printf("inserting %d at position %d, size=%d\n", k--,h,myQ->getSize(myQ));
    }
    printf("Iserted %d elements\n", myQ->getSize(myQ));

    for (j = 0; j<QUEUE_SIZE; j++)
    {
        t=myQ->tail;
        k=myQ->remove(myQ);
        printf("Removing %d at position %d, size=%d\n", k, t, myQ->getSize(myQ));
    }
    printf("Last item removed = %d\n", k);

    printf("Current queue size %d\n", myQ->getSize(myQ));
    puts("Queue test program");

    return 0;
}
//--------------------- 
//作者：Into_Wang 
//来源：CSDN 
//原文：https://blog.csdn.net/Into_Wang/article/details/54882365 
//版权声明：本文为博主原创文章，转载请附上博文链接！
