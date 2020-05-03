#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ckl_debug.h"
#include "ckl_hashtable.h"

#define NAME_LEN (32)
#define PHONE_LEN (12)
typedef struct{
    int id;
    int age;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
} employee_t;


int main(int argc,char *argv[])
{
    hashtable_t *hashtable = NULL;

    srand(time(NULL));

    hashtable = hash_table_create(HASH_TABLE_SLOT_SIZE,SIAMPLE_HASH);

    int i=0;
    for(i=0;i<5;i++){
        employee_t *employee = (employee_t*)malloc(sizeof(employee_t));
        employee->id = rand()%1000+1;
        employee->age = rand()%50+18;
        snprintf(employee->name,sizeof(employee->name),"M04304%04d",employee->id);
        snprintf(employee->phone,sizeof(employee->phone),"093387%04d",employee->id);

        DEBUG("id = %4d , age = %2d , name = %s , phone = %s",employee->id,employee->age,employee->name,employee->phone);

        //hashtable->hashtable_insert(hashtable,(void *)employee->name,(void *)employee);
        hashtable->hashtable_insert(hashtable,(void *)&employee->id,(void *)employee);
    }

    hashtable->hashtable_destory(hashtable);
    return 0;
}