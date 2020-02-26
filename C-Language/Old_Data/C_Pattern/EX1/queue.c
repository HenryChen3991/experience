#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void Queue_Init(QUEUE* me, int (*isFullFunction)(QUEUE* const me),
    int (*isEmptyFunction)(QUEUE* const me),
    int (*getSizeFunction)(QUEUE* const me),
    void (*insertFunction)(QUEUE* const me, int k),
    int (*removeFunction)(QUEUE* const me)){

    me->head=0;
    me->tail=0;
    me->size=0;

    me->isFull=isFullFunction;
    me->isEmpty=isEmptyFunction;
    me->getSize=getSizeFunction;
    me->insert=insertFunction;
    me->remove=removeFunction;
}

void Queue_Cleanup(QUEUE* const me){


}

int Queue_isFull(QUEUE* const me){

    return (me->head+1)%QUEUE_SIZE==me->tail;
}

int Queue_isEmpty(QUEUE* const me){

    return me->head==me->tail;
}

int Queue_getSize(QUEUE* const me){

    return me->size;
}

void Queue_insert(QUEUE* const me, int k){

    if (!me->isFull(me))
    {
        me->buffer[me->head]=k;
        me->head=(me->head+1)%QUEUE_SIZE;
        ++me->size;
    }
}

int Queue_remove(QUEUE* const me){

    int value=-9999;

    if(!me->isEmpty(me))
    {
        value=me->buffer[me->tail];
        me->tail=(me->tail+1)%QUEUE_SIZE;
        --me->size;
    }

    return value;
}

QUEUE* Queue_Create(void){

    QUEUE* me=(QUEUE*)malloc(sizeof(QUEUE));

    if (me!=NULL)
    {
        Queue_Init(me,Queue_isFull,Queue_isEmpty,Queue_getSize,
            Queue_insert,Queue_remove);
    }

    return me;
}

void Queue_Destroy(QUEUE* const me){

    if (me!=NULL)
    {
        Queue_Cleanup(me);
    }

    free(me);
}
//--------------------- 
//作者：Into_Wang 
//来源：CSDN 
//原文：https://blog.csdn.net/Into_Wang/article/details/54882365 
//版权声明：本文为博主原创文章，转载请附上博文链接！
