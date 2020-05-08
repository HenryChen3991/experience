#ifndef CKL_HASHTABLE_H
#define CKL_HASHTABLE_H

/**

ref:
linux-4.20.12\security\selinux\ss\hashtab.c
linux-4.20.12\security\selinux\ss\hashtab.h

h(Key)
[0]------------->[itemA]->[itemC]
[1]------------->[ NULL]
[2]------------->[itemB]
[3]------------->[ NULL]
[4]------------->[itemD]->[itemE]->[itemF]
 |                  |
 |                  |
 |                  |
slot_size           |
                    |
                bucket_size = 6 (A~F)
**/

#define HASH_TABLE_SLOT_SIZE (64)

typedef enum
{
    SIAMPLE_HASH,
    TIME33_HASH
} hash_func_e;

typedef struct hashtable_node
{
    const void *  key;
    void *        data;
    struct hashtable_node * next;
//    struct hashtable_node * prev;
} hashtable_node_t;

typedef struct hashtable
{
    hashtable_node_t **htables;                                             //hash table
    unsigned int       slot_size;                                           //number of slots in hash table
    unsigned int       bucket_size;                                         //number of elements in hash table
    hash_func_e        hash_func_control;                                   //control using which hash function
    int (*hash_fun)(struct hashtable *ht,const void *key);                  //hashtable used hash function
    int (*keycmp)(const void *key1,const void *key2);                       //key comparison function
    int (*hashtable_insert)(struct hashtable *ht,void *key,void *data);     //insert data into hashtable
    int (*hashtable_dump)(struct hashtable *ht);                            //dump hashtable
    int (*hashtable_search)(struct hashtable *ht,void *key,void **ptr);     //search hashtable_node from hashtable
    int (*hashtable_delete)(struct hashtable *ht,void *key);                //delete hashtable_node from hashtable
    int (*hashtable_revise)(struct hashtable *ht,void *key,void *data);     //revise hashtable_node from hashtable
    unsigned int (*hashtable_destory)(struct hashtable *ht);                //free hashtable
    unsigned int (*hash_node_free)(struct hashtable_node *htNode);          //free hashtable_node
} hashtable_t;

hashtable_t * hash_table_create(unsigned int slot_size,hash_func_e hash_func_control);

#endif //CKL_HASHTABLE_H
