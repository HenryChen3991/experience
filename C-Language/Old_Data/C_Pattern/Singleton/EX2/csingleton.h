#ifndef CSINGLETON_H
#define CSINGLETON_H
 
#include <stdlib.h>
typedef struct {
    int value;
} _Node;

typedef struct {
    void* (*ctor)(void *_self);
    void* (*dtor)(void *_self);
    void* (*createInstance)(void *self);
    void *instance;
} _CSingleton;
 
extern const void *CSingleton;
 
void *GetInstance(void);
 
#endif
