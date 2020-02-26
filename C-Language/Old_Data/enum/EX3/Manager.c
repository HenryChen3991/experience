#include <stdio.h>
#include <stdlib.h>
#include "Manager.h"
static int OLT_TYPE = Nokia_7360_OLT;


rOltPrototype_t R_OltPrototype = {
	{
		{ {0x2c,0xfa,0xa2} },
		{ {0xc8,0xf8,0x6d} }
	}
};

sumitomoOltPrototype_t Sumitomo_OltPrototype = {
	{
                { {0xaa,0xaa,0xaa} },
                { {0xf4,0xd6,0xa0} },
		{ {0xcc,0xcc,0xcc} }
        }
};

nokia7360OltPrototype_t Nokia_7360_OltPrototype = {
	{
		{ {0x00,0x00,0x01} },
	}
};

olt_prototype_t olt = {
	&R_OltPrototype,
	&Sumitomo_OltPrototype,
	&Nokia_7360_OltPrototype
};


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


void print_mac(char *str)
{
  printf("Henry-- str = %2hhx:%2hhx:%2hhx\n",*str,*(str+1),*(str+2));
  //printf("Henry-- str = %2hhx:%2hhx:%2hhx\n",str,(str+1),(str+2)); //error
}

int compare_mac(char *str,char *target)
{
	int ret = -1;
	return memcmp(str,target,sizeof(oltPrototype_t));
}

void new_check(char *str)
{
	int size = 0;
	int ret = -1;
	int i = 0;
	printf("**************************************\n");
	printf("new check method\n");
	printf("**************************************\n");
	print_current_olt_prototype();
	if(OLT_TYPE != No_Connect_OLT){
		size = sizeof(R_OltPrototype)/sizeof(oltPrototype_t);
		for(i = 0 ; i < size ; i++)
		{
			print_mac(olt.nokia_r->olt_prototype[i].olt_mac);
			if( compare_mac(str,olt.nokia_r->olt_prototype[i].olt_mac) == 0){
				OLT_TYPE = Nokia_R_OLT;
				return ;
			}	
		}
	
		size =  sizeof(Sumitomo_OltPrototype)/sizeof(oltPrototype_t);	
        	for(i = 0 ; i < size ; i++)
        	{
			print_mac(olt.sumitomo->olt_prototype[i].olt_mac);
			if(  compare_mac(str,olt.sumitomo->olt_prototype[i].olt_mac) == 0){
				printf("equal 0\n");
				OLT_TYPE = Sumitomo_OLT;
				return ;
			}
        	}

		size = sizeof(Nokia_7360_OltPrototype)/sizeof(oltPrototype_t);
		for(i = 0 ; i < size ; i++)
        	{
			print_mac(olt.nokia_7360->olt_prototype[i].olt_mac);
			if(  compare_mac(str,olt.nokia_7360->olt_prototype[i].olt_mac) == 0){
				OLT_TYPE = Nokia_7360_OLT;
				return ;
			}
		}
		OLT_TYPE = Unknown_OLT;
	}
}


