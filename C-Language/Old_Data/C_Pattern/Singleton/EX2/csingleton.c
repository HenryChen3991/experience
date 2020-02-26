#include "csingleton.h"
#include <stdlib.h>
 
static void *csingletonCtor(void *_self) {
    _CSingleton *self = _self;

    self->instance = _self;
//    self->instance = (_Node *)malloc(sizeof(_Node));
//    ((_Node)self->instance)->value = 5;
    return self;
}
 
static void *csingletonDtor(void *_self) {
    _CSingleton *self = _self;
 
    self->instance = NULL;
 
    return self;
}
 
static void *csingletonCreateInstance(void *_self) {
    _CSingleton *self = _self;
 
    self->instance = _self;
 
    return self;
}
 
static _CSingleton _csingleton = {
    csingletonCtor, csingletonDtor, csingletonCreateInstance, NULL
};
 
const void *CSingleton = &_csingleton;
 
void *GetInstance(void) {
    if (NULL == ((_CSingleton*)CSingleton)->instance) {
        return csingletonCtor((void *)CSingleton);
    } else {
        return ((_CSingleton*)CSingleton)->instance;
    }
}
