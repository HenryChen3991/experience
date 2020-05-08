#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ckl_debug.h"
#include "ckl_hashtable.h"
#include "ckl_ht.h"

/*
 * HASH TABLE BEAHVIOR
 */
hashtable_t * hash_table_create(unsigned int slot_size,hash_func_e hash_func_control);
int hash_table_insert(struct hashtable *ht,void *key,void *data);
int hashtable_dump(struct hashtable *ht);
int hashtable_search(struct hashtable *ht,void *key,void **ptr);
int hashtable_delete(struct hashtable *ht,void *key);
int hashtable_revise(struct hashtable *ht,void *key,void *data);
unsigned int hash_table_destory(struct hashtable *ht);
unsigned int hash_node_free(struct hashtable_node *htNode);
int keycmp(const void *key1,const void *key2);

#define CHECK_ARGUMENT(arg) \
    if(arg == NULL){ \
    ERROR("Invalid arguments"); \
    return INVALID_ARGUMENTS; \
}

/*
 * HASH FUNCTION HANDLER AND COLLECT HASH FUNCTION
 */
int hash_func_handler(struct hashtable *ht,const void *key);
unsigned int siample_hash(const char *str);
unsigned int hash_33(const char* key);

hashtable_t * hash_table_create(unsigned int slot_size,hash_func_e hash_func_control)
{
    hashtable_t *ht = NULL;
    int i = 0;

    PRINT_FUNC();

    if(slot_size == 0){
        ERROR("invalid arguments");
        exit(0);
    }

    ht = (hashtable_t *)malloc(sizeof(hashtable_t));
    if(ht == NULL){
        ERROR("hash table malloc failed");
        exit(0);
    }

    ht->htables = (hashtable_node_t **)malloc(slot_size * sizeof(hashtable_node_t*));
    if(ht->htables == NULL){
        ERROR("hash table's htables malloc failed");
        exit(0);
    }

    //assign ht
    ht->bucket_size = 0;
    ht->slot_size = slot_size;
    ht->hash_func_control = hash_func_control;
    ht->hash_fun = hash_func_handler;
    ht->hashtable_destory = hash_table_destory;
    ht->hash_node_free = hash_node_free;
    ht->keycmp = keycmp;
    ht->hashtable_insert = hash_table_insert;
    ht->hashtable_dump = hashtable_dump;
    ht->hashtable_search = hashtable_search;
    ht->hashtable_delete = hashtable_delete;

    //initial ht->htables
    for(i = 0; i < slot_size ; i++)
        ht->htables[i] = NULL;

    return ht;
}
int hashtable_dump(struct hashtable *ht)
{
    int i = 0;
    hashtable_node_t *cur = NULL;
    hashtable_node_t *tmp = NULL;
    SET_COLOR(LIGHT_GREEN);
    PRINT_FUNC();
    for(i = 0; i < ht->slot_size ; i++){
        if(ht->htables[i] != NULL){
            cur = ht->htables[i];
            printf("%20s:%4d ht[%03d]->",__FILE__,__LINE__,i);
            while(cur!=NULL){
                printf("%s->",(char *)cur->key);
                cur = cur->next;
            }
            printf("NULL\n");
        }
    }
    SET_COLOR(NONECOLOR);

#if 0
    //check cur-> data whether can cast to employee_t type
    cur = ht->htables[0];
    employee_t *employee = (employee_t*)cur->data;
    DEBUG("id = %d , age = %d , name = %s , phone = %s",employee->id,employee->age,employee->name,employee->phone);
#endif
}

int hashtable_search(struct hashtable *ht,void *key,void **ptr)
{
    int idx = -1;
    hashtable_node_t *cur = NULL;
    if(key == NULL){
        ERROR("Invalid arguments");
        return INVALID_ARGUMENTS;
    }
    idx = ht->hash_fun(ht,key);
    cur = ht->htables[idx];
    if(cur != NULL){
        while(cur != NULL){
            if(ht->keycmp(cur->key,key)==0){
                //may revise method, pass a pointer pointer to store the cur->data;
                //so that we can support diffenert data type in the furture.
                *ptr = cur->data;
                return SUCCESS;
            }
            cur = cur->next;
        }
        return NOT_FOUND;
    }
    else{
        return NOT_FOUND;
    }
}

int hashtable_delete(struct hashtable *ht,void *key)
{
    CHECK_ARGUMENT(key);

    int idx = -1;
    hashtable_node_t *cur = NULL;
    hashtable_node_t *prev = NULL;

    idx = ht->hash_fun(ht,key);
    cur = ht->htables[idx];

    while(cur != NULL){
        if(ht->keycmp(cur->key,key)==0){
            if(prev != NULL){
                prev->next = cur->next;
            }
            else{
                ht->htables[idx] = NULL;
            }
            ht->hash_node_free(cur);
            return SUCCESS;
        }
        prev = cur;
        cur = cur->next;
    }

    return NOT_FOUND;
}

int hashtable_revise(struct hashtable *ht,void *key,void *data)
{

}

unsigned int hash_table_destory(struct hashtable *ht)
{
    int i = 0;
    hashtable_node_t *cur = NULL;
    hashtable_node_t *tmp = NULL;

    PRINT_FUNC();

    for(i = 0 ; i < ht->slot_size ; i++){
        if(ht->htables[i] != NULL){
            cur = ht->htables[i];
            while(cur != NULL){
                tmp = cur;
                cur = cur->next;
                ht->hash_node_free(tmp);
            }
            ht->htables[i] = NULL;
        }
    }

    free(ht->htables);
    free(ht);

    return SUCCESS;
}

unsigned int hash_node_free(struct hashtable_node *htNode)
{
    //1.need to cast it to a non-const pointer. because "free" takes a "void *", not a "const void*"
    //2.Due to htNode->key is pointer to htNode->data so cannot free two twice. ohterwise will segmentation fault
    //free((void*)htNode->key);

    free(htNode->data);
    free(htNode);
    return SUCCESS;
}

int keycmp(const void *key1,const void *key2)
{
    char emptyStr = '\0';
    int len = 0;
    const char *key1_ptr = NULL;
    const char *key2_ptr = NULL;
    key1_ptr = key1;
    key2_ptr = key2;

   // DEBUG("%d %d %d %d",*(int*)key1,*(int*)key2,strlen(key1_ptr),strlen(key2_ptr));
    if(key1_ptr == NULL)
        key1_ptr = &emptyStr;
    if(key2_ptr == NULL)
        key2_ptr = &emptyStr;

    return strncmp(key1_ptr,key2_ptr,strlen(key2_ptr));
}

int hash_table_insert(struct hashtable *ht,void *key,void *data)
{
    hashtable_node_t *cur = NULL;
    hashtable_node_t *new = NULL;
    hashtable_node_t *tmp = NULL;
    int i = 0;
    int idx = -1;

    if( ht == NULL || key == NULL || data == NULL){
        ERROR("invalid arguments");
        return INVALID_ARGUMENTS;
    }

    idx = ht->hash_fun(ht,key);
//    DEBUG_COLOR(BLUE,"idx = %d",idx);

    cur = ht->htables[idx];

#if 0
    hashtable_node_t *prev = NULL;
    //find insert position (small to big order)
    while( (cur!=NULL) && ( ht->keycmp(key,cur->key) > 0) ){
        DEBUG("small");
        prev = cur;
        cur = cur->next;
    }
#endif

    if( (cur!=NULL) && ( ht->keycmp(key,cur->key) == 0) ){
        ERROR("The data had existed");
        return INTERNAL_ERROR;
    }

    new = (hashtable_node_t *)malloc(sizeof(hashtable_node_t));
    if(new == NULL){
        ERROR("hash node malloc failed");
        return INTERNAL_ERROR;
    }
    //assign hash node
    new->key = key;
    new->data = data;
    new->next = NULL;

    if(cur!=NULL){
        tmp = cur->next;
        cur->next = new;
        new->next = tmp;
    }
    else{
        ht->htables[idx] = new;
    }

    ht->bucket_size++;

    return SUCCESS;
}

/*
 * HASH FUNCTION HANDLER AND COLLECT HASH FUNCTION
 */

int hash_func_handler(struct hashtable *ht,const void *key)
{
    switch (ht->hash_func_control)
    {
        case SIAMPLE_HASH:
            return (siample_hash(key)%ht->slot_size);
        case TIME33_HASH:
            return (hash_33(key)%ht->slot_size);
        default:
            ERROR("Method not supported");
            exit(0);
    }
}
unsigned int siample_hash(const char *str)
{
	register unsigned int hash = 0;
	register unsigned int seed = 131;

	while(*str)
	{
		hash = hash*seed + *str++;
	}

	return hash & (0x7FFFFFFF);
}

unsigned int hash_33(const char* key)
{
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + hash + *key++;
    }
    return hash;
}
