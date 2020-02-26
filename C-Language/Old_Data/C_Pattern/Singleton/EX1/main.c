#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h> 

typedef struct ID{
    char *name;
    int score;    
} id;
 
static id * _id = NULL;
static pthread_mutex_t pMutex = PTHREAD_MUTEX_INITIALIZER;
 
id *getInstance(){
 
    pthread_mutex_lock( &pMutex ); // 上鎖
    if(_id != NULL) {
        pthread_mutex_unlock( &pMutex ); // 解鎖
        return _id;
    } else {
 
        _id = (id*)malloc(sizeof(id));
        assert(_id != NULL);
        pthread_mutex_unlock( &pMutex ); // 解鎖
        return _id;
    }
}
 
int main(){
 
    id * i1, *i2;
 
    #pragma omp parallel
    {
        i1 = getInstance() ;
        i1->name = "Rong";
        i1->score = 1;
    }
    #pragma omp parallel 
    {
        i2 = getInstance() ;
        i2->name = "Tao";
    }
 
    if(i1 == i2)
        fprintf(stdout, " i1 == i2 \n");
 
    fprintf(stdout, "i1->name = %s, i1->score = %d\n",i1->name, i1->score);
    fprintf(stdout, "i2->name = %s, i2->score = %d\n",i2->name, i2->score);
 
    return 0 ;
}
