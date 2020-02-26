#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 10

typedef struct queue
{
    int buffer[QUEUE_SIZE];
    int head;
    int size;
    int tail;

    int (*isFull)(struct queue* const me);
    int (*isEmpty)(struct queue* const me);
    int (*getSize)(struct queue* const me);
    void (*insert)(struct queue* const me, int k);
    int (*remove)(struct queue* const me);
}QUEUE;

void Queue_Init(QUEUE* me, int (*isFullFunction)(QUEUE* const me),
    int (*isEmptyFunction)(QUEUE* const me),
    int (*getSizeFunction)(QUEUE* const me),
    void (*insertFunction)(QUEUE* const me, int k),
    int (*removeFunction)(QUEUE* const me));
void Queue_Cleanup(QUEUE* const me);

int Queue_isFull(QUEUE* const me);
int Queue_isEmpty(QUEUE* const me);
int Queue_getSize(QUEUE* const me);
void Queue_insert(QUEUE* const me, int k);
int Queue_remove(QUEUE* const me);

QUEUE* Queue_Create(void);
void Queue_Destroy(QUEUE* const me);

#endif
//--------------------- 
//作者：Into_Wang 
//来源：CSDN 
//原文：https://blog.csdn.net/Into_Wang/article/details/54882365 
//版权声明：本文为博主原创文章，转载请附上博文链接！
