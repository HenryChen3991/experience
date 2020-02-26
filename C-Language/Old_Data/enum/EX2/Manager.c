#include <stdio.h>
#include <stdlib.h>
#include "Manager.h"
static int OLT_TYPE = Nokia_7360_OLT;

oltPrototype_t oltPrototype[oltPrototypeNum] = {
    { {0xf3,0xd5,0xa1} },
    { {0xf4,0xd6,0xa0} }, //Sumitomo_OLT
    { {0x00,0x00,0x01} },
};
int getOltType(void)
{
	return OLT_TYPE;
}
int setOltType(olt_type_e type)
{
	OLT_TYPE = type;
	return 1;
}

void print_current_olt_prototype(void){
    switch(OLT_TYPE){
        case Nokia_7360_OLT:
            printf("Current olt_prototype is Nokia_7360_OLT\n");
            break;
        case Sumitomo_OLT:
            printf("Current olt_prototype is Sumitomo_OLT\n");
            break;
        case Nokia_R_OLT:
            printf("Current olt_prototype is Nokia_R_OLT\n");
            break;
        case Unknown_OLT:
            printf("Current olt_prototype is Unknown_OLT\n");
            break;
        case No_Connect_OLT:
            printf("Current olt_prototype is No_Connect_OLT\n");
            break;
    }
}
int check_olt_prototype(char *str)
{
    print_current_olt_prototype();
    int ret = -1;
    int i = 0;
    //printf("str = %2hhx:%2hhx:%2hhx\n",*str,*(str+1),*(str+2));
    if(OLT_TYPE != No_Connect_OLT){
        for(i=0;i<oltPrototypeNum;i++){
            ret = memcmp(str,oltPrototype[i].olt_mac,sizeof(oltPrototype_t));
            if(ret == 0){
                OLT_TYPE = i;
                return ret;
            }
            ret = -1;
        }
        if(ret == -1)
            OLT_TYPE = Unknown_OLT;
    }
    return ret;
}

