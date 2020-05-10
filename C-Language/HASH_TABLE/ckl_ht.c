#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ckl_debug.h"
#include "ckl_hashtable.h"
#include "ckl_util.h"
#include "ckl_ht.h"

#define BUF_LEN (2048)
#define SAMPLE_SIZE (20)
int main(int argc,char *argv[])
{
    hashtable_t *hashtable = NULL;
    Ret ret = INITIALIZATION;

    srand(time(NULL));

    //produce random unique numbers
    int arr[BUF_LEN]={0};
    int n = sizeof(arr)/ sizeof(arr[0]);
    initArray(&arr[0], n);
    randomize(arr, n);

    //check SAMPLE_SIZE vs BUF_LEN
    if(SAMPLE_SIZE > BUF_LEN){
        ERROR("Uncorrect SAMPLE_SIZE");
        exit(0);
    }

    //create hashtable
    hashtable = hash_table_create(HASH_TABLE_SLOT_SIZE,SIAMPLE_HASH);

    //produce testing data and insert into hashtable
    int i=0;
    for(i=0;i<SAMPLE_SIZE;i++){
        //DEBUG_COLOR(LIGHT_GREEN,"produce sample %d:",i);
        employee_t *employee = (employee_t*)malloc(sizeof(employee_t));
        employee->id = arr[i];
        employee->age = rand()%50+18;
        snprintf(employee->name,sizeof(employee->name),"M04304%04d",employee->id);
        snprintf(employee->phone,sizeof(employee->phone),"093387%04d",employee->id);
        DEBUG("id = %4d , age = %2d , name = %s , phone = %s",employee->id,employee->age,employee->name,employee->phone);
#if 1
        //if insert fail due to have the same key, need to free the employee, then produce next testing data;
        HASHTABLE_INSET(hashtable,employee,name);
        //HASHTABLE_INSET(hashtable,employee,id);
#endif //0
    }

    //dump hashtable
    hashtable->hashtable_dump(hashtable);

    //search hashtable example
    char name[NAME_LEN]={0};
    employee_t *employee_ptr = NULL;
    snprintf(name,sizeof(name),"M04304%04d",arr[2]);
    HASHTABLE_SEARCH(hashtable,name,employee_ptr);
    snprintf(name,sizeof(name),"M04304%04d",arr[30]);
    HASHTABLE_SEARCH(hashtable,name,employee_ptr);

    //delete hashtable_node_t
    HASHTABLE_DELETE(hashtable,name);

    //dump hashtable
    hashtable->hashtable_dump(hashtable);

    //revise hashtable someone hashtable_node_t
    employee_t *employee = (employee_t*)malloc(sizeof(employee_t));
    employee->id = arr[10];
    employee->age = rand()%50+18;
    snprintf(employee->name,sizeof(employee->name),"M04304%04d",employee->id);
    snprintf(employee->phone,sizeof(employee->phone),"094447%04d",employee->id);
    DEBUG("new node id = %4d , age = %2d , name = %s , phone = %s",employee->id,employee->age,employee->name,employee->phone);
    hashtable->hashtable_revise(hashtable,(void *)employee->name,(void *)employee);
    snprintf(name,sizeof(name),"M04304%04d",arr[10]);
    HASHTABLE_SEARCH(hashtable,name,employee_ptr);

    //destory hashtable
    hashtable->hashtable_destory(hashtable);
    return 0;
}