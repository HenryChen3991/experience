#ifndef CKL_HT_H
#define CKL_HT_H
#define NAME_LEN (32)
#define PHONE_LEN (12)

typedef struct{
    int id;
    int age;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
} employee_t;

#define DUMP_EMPLOYEE(ptr) DEBUG("id = %04d , age = %2d , name = %s , phone = %s",ptr->id,ptr->age,ptr->name,ptr->phone);

#define HASHTABLE_SEARCH(hashtable,name,ptr) \
    DEBUG("ready to search for %s ...",name); \
    if( (ret=hashtable->hashtable_search(hashtable,(void *)name,(void *)&ptr)) != SUCCESS ){ \
        DEBUG("not found!!!"); \
    }else{ \
        DEBUG("found it!!!"); \
        DUMP_EMPLOYEE(ptr); \
    }

#define HASHTABLE_DELETE(hashtable,name) \
    DEBUG("ready to delete hashtable_node key = %s ...",name); \
    if( (ret = hashtable->hashtable_delete(hashtable,name) ) == SUCCESS ){ \
        DEBUG("delete successfully."); \
    } \
    else if (ret == NOT_FOUND){ \
        DEBUG("can't delete this node because can't found it in the hashtable."); \
    }

#endif //CKL_HT_H
