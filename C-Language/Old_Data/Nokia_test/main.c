#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
//extern E2PROM_DATA_MAP nokia_ri[];

void main(int argc,char *argv[])
{
/*
    printf("%s\n",nokia_ri[0].parm);
    nokia_ri[0].set(argv[1],nokia_ri[0].initialAddr,nokia_ri[0].length);

    printf("ret = %d\n",find_nokia_RI_entry("Format"));
    printf("ret = %d\n",find_nokia_RI_entry("Format01"));

    if(find_nokia_RI_entry("Format")>=0){
        printf("support\n");
    }

    if(find_nokia_RI_entry("MACAddress")>0){
        printf("support\n");
    }
*/
    if(argv[3]==NULL)
	printf("argv[3] is null\n");
    else
	printf("argv[3] not null\n");
    print_all_support_parm();

    int index = find_nokia_RI_entry(argv[1]);
    char gtk_block[6] = {'2','2','0','0','0','0'};
    int len = 5;
    printf("index = %d, argv len = %d\n",index,strlen(argv[3]));
    printf("initialAddr = 0x%x\n",nokia_ri[index].initialAddr);  
    printf("in main() gtk_block[0]=%c ,0x%x\n",gtk_block[0],gtk_block[0]);   
    nokia_ri[index].set(index,&argv[3][0],gtk_block,&len);
    printf("after setting, len = %d, gtk_block[0] = %c , 0x%x\n",len,gtk_block[0],gtk_block[0]);

//    int daddress = strtol(&nokia_ri[index].initialAddr, NULL, 0);
    int daddress = nokia_ri[index].initialAddr;
    printf("daddress = 0x%x\n",daddress);

/*
    char str[] = "0x3e";
    daddress = strtol(str, NULL, 0);
    printf("daddress = 0x%x\n",daddress);
*/
  
    
}
