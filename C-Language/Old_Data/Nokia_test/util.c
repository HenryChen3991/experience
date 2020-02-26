#include <stdio.h>
#include <string.h>
#include "util.h"
/*
typedef struct {
    char parm[32];
    unsigned char initialAddr;
    int length;
    int (*set)(char *,unsigned char ,int);
} E2PROM_DATA_MAP;
*/

//int set_Format(char *,unsigned char,int );
int set_MACAddress(int,char *,char *,int *);
//void print_all_support_parm(void);
//int find_nokia_RI_entry(char *);

E2PROM_DATA_MAP nokia_ri[] = {
    {"Format"            ,0x0   ,2  ,/*set_Format*/},   //0x56 i2c-device (256 bytes)
    {"MfrID"             ,0x2   ,4  ,NULL},
    {"FactoryCode"       ,0x6   ,2  ,NULL},
    {"HardwareVersion"   ,0x8   ,12 ,NULL},
    {"ICS"               ,0x14  ,2  ,NULL},
    {"YPSerialNum"       ,0x16  ,16 ,NULL},
    {"CleiCode"          ,0x26  ,10 ,NULL},
    {"Mnemonic"          ,0x30  ,8  ,NULL},
    {"ProgDate"          ,0x38  ,6  ,NULL},
    {"MACAddress"        ,0x3e  ,6  ,set_MACAddress},
    {"DeviceIDPref"      ,0x44  ,2  ,NULL},
    {"SWImage"           ,0x46  ,2  ,NULL},
    {"OnuMOde"           ,0x48  ,2  ,NULL},
    {"Mnemonic2"         ,0x4a  ,4  ,NULL},
    {"Password"          ,0x4e  ,10 ,NULL},
    {"G984Serial"        ,0x58  ,4  ,NULL},
    {"HWConfiguration"   ,0x5c  ,8  ,NULL},
    {"PartNumber"        ,0x64  ,10 ,NULL},
    {"Variant"           ,0x6e  ,2  ,NULL},
    {"Spare4"            ,0x70  ,12 ,NULL},
    {"Checksum"          ,0x7c  ,2  ,NULL},
    {"InserviceReg"      ,0x7e  ,2  ,NULL},
    {"UserNmae"          ,0x80  ,16 ,NULL},
    {"UserPassword"      ,0x90  ,8  ,NULL},
    {"MgntUserName"      ,0x98  ,16 ,NULL},
    {"MgntUserPasswrod"  ,0xA8  ,8  ,NULL},
    {"SSID_1_Name"       ,0xB0  ,16 ,NULL},
    {"SSID_1_Password"   ,0xC0  ,8  ,NULL},
    {"SSID_2_Name"       ,0xC8  ,16 ,NULL},
    {"SSID_2_Password"   ,0xD8  ,8  ,NULL},
    {"OperatorID"        ,0xE0  ,4  ,NULL},
    {"SLID"              ,0xE4  ,16 ,NULL},
    {"CountryID"         ,0xF4  ,2  ,NULL},
    {"GroupID"           ,0xF6  ,4  ,NULL},
    {"Spare5"            ,0xFA  ,2  ,NULL},
    {"Checksum1"         ,0xFC  ,2  ,NULL},
    {"Spare6"            ,0xFE  ,2  ,NULL},
    {"RollbackFlag"      ,0x0   ,2  ,NULL},   //0x57 i2c-device
};

int get_e2prom_data_map_length(){
    return (sizeof(nokia_ri)/sizeof(E2PROM_DATA_MAP));
}

void print_all_support_parm(void){
    int e2prom_data_len = get_e2prom_data_map_length();
    int i = 0;
    printf("Support parm:\n");
    for(i = 0 ; i < e2prom_data_len ; i++){
        printf("%16s\t",nokia_ri[i].parm);
        if(  (i+1)%4==0  || i==(e2prom_data_len-1) )
            printf("\n");
    }
}

int find_nokia_RI_entry(char *parm){
    int e2prom_data_len = get_e2prom_data_map_length();
    int i = 0;
    int ret = RET_FAILED;
    for(i = 0; i < e2prom_data_len ;i++){
        if(!strncmp(nokia_ri[i].parm,parm,E2PROM_PARM_LEN)) {
            ret = i;
            break;
        }
    }
    return ret;
}


int set_Format(char *data,unsigned char addr, int len){

    printf("data = %s, addr = 0x%x, len = %d\n",data,addr,len);
    printf("data = %c\n",data[0]);
    printf("data = 0x%x\n",data[0]);
}

int parseMacAddr(char *str){ 
    int ret = 1;
    printf("parse Mac addr\n");
    printf("mac = %s\n",str);
    while( *str ){
       if(*str!=':'){
            printf("%c",*str);   
            if((*str >= '0') && (*str <= '9'))      ret = 1;
            else if((*str >= 'a') && (*str <= 'f')) ret = 1;
            else if((*str >= 'A') && (*str <= 'F')) ret = 1;
            else 
               ret = -1;
       }
       str++;
    }
    printf("\nret = %d\nparse end\n",ret);
    return ret;
}
int set_MACAddress(int index,char *data,char *gtk_block,int *len){
    printf("index = %d, data = %s\n",index,data);
    printf("len = %d\n",*len);
    printf("gtk_block[0] = %c\n",gtk_block[0]);
    unsigned char mac[6] = {0}; 
    gtk_block[0] = 0x33;
    if( strlen(data) == 17 )
        if( parseMacAddr(data) == 1){
             int i = 0;
             sscanf(data,"%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
             for(i = 0 ; i < 6 ; i++){
                 printf("0x%x\n",mac[i]);
                 gtk_block[i] = mac[i];
             }
             *len = 6;
        }
    else
        printf("not valid\n");
   
}
