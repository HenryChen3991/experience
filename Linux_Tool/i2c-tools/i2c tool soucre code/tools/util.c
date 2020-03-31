/*
    util.c - helper functions
    Copyright (C) 2006-2009  Jean Delvare <jdelvare@suse.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/

#include <stdio.h>
//#ifdef ODM_GEMTEK
#include <stdlib.h>
#include <string.h>
//#endif //ODM_GEMTEK
#include "util.h"

/* Return 1 if we should continue, 0 if we should abort */
int user_ack(int def)
{
    char s[2];
    int ret;

    if (!fgets(s, 2, stdin))
        return 0; /* Nack by default */

    switch (s[0]) {
        case 'y':
        case 'Y':
            ret = 1;
            break;
        case 'n':
        case 'N':
            ret = 0;
            break;
        default:
            ret = def;
    }

    /* Flush extra characters */
    while (s[0] != '\n') {
        int c = fgetc(stdin);
        if (c == EOF) {
            ret = 0;
            break;
        }
        s[0] = c;
    }

    return ret;
}

//#ifdef ODM_GEMTEK
void set_MACAddress(char *,char *,int *);
void set_MACAddress_help(void);
int parse_mac_addr(char *);
void set_ProgDate(char*,char *,int*);
void set_ProgDate_help(void);
int parse_prog_date(char *);
void set_ICS(char *,char *,int *);
void set_ICS_help(void);
int parse_ics(char *);
void set_HardwareVersion(char *,char *,int *);
int parse_hardware_version(char *);
void set_HardwareVersion_help(void);
void set_YPSerialNum(char *,char *,int *);
void set_YPSerialNum_help(void);
int parse_yp_serialnum(char *);
void set_FactoryCode(char *,char *,int *);
void set_FactoryCode_help(void);
int parse_factorycode(char *);

#define MAC_ADDRESS_LEN        6
#define MAC_STR_ADDRESS_LEN   17
#define PROG_DATE_LEN          6
#define HARDWARE_VERSION_LEN  12
#define YPSERIALNUM_LEN       16
#define ICS_LEN                2
#define FACTORY_CODE_LEN       2

E2PROM_DATA_MAP nokia_ri[] = {
    {"Format"            ,0x0   ,2  ,CHARACTER_MODE   ,NULL},//0x56 i2c-device (256 bytes)
    {"MfrID"             ,0x2   ,4  ,CHARACTER_MODE   ,NULL},
    {"FactoryCode"       ,0x6   ,2  ,CHARACTER_MODE   ,set_FactoryCode},
    {"HardwareVersion"   ,0x8   ,12 ,CHARACTER_MODE   ,set_HardwareVersion},
    {"ICS"               ,0x14  ,2  ,CHARACTER_MODE   ,set_ICS},
    {"YPSerialNum"       ,0x16  ,16 ,CHARACTER_MODE   ,set_YPSerialNum},//0x20 *8 ,0x30 *8
    {"CleiCode"          ,0x26  ,10 ,CHARACTER_MODE   ,NULL},
    {"Mnemonic"          ,0x30  ,8  ,CHARACTER_MODE   ,NULL},
    {"ProgDate"          ,0x38  ,6  ,CHARACTER_MODE   ,set_ProgDate},
    {"MACAddress"        ,0x3e  ,6  ,HEX_MODE         ,set_MACAddress},
    {"DeviceIDPref"      ,0x44  ,2  ,HEX_MODE         ,NULL},
    {"SWImage"           ,0x46  ,2  ,HEX_MODE         ,NULL},
    {"OnuMOde"           ,0x48  ,2  ,HEX_MODE         ,NULL},//unknown
    {"Mnemonic2"         ,0x4a  ,4  ,CHARACTER_MODE   ,NULL},//unknown, ref on Mnemonic
    {"Password"          ,0x4e  ,10 ,HEX_MODE         ,NULL},
    {"G984Serial"        ,0x58  ,4  ,HEX_MODE         ,NULL},
    {"HWConfiguration"   ,0x5c  ,8  ,HEX_MODE         ,NULL},
    {"PartNumber"        ,0x64  ,12 ,CHARACTER_MODE   ,NULL},
    {"Variant"           ,0x6e  ,2  ,CHARACTER_MODE   ,NULL},
    {"Spare4"            ,0x70  ,12 ,HEX_MODE         ,NULL},
    {"Checksum"          ,0x7c  ,2  ,HEX_MODE         ,NULL},
    {"InserviceReg"      ,0x7e  ,2  ,HEX_MODE         ,NULL},
    {"UserName"          ,0x80  ,16 ,CHARACTER_MODE   ,NULL},//*
    {"UserPassword"      ,0x90  ,8  ,CHARACTER_MODE   ,NULL},//*
    {"MgntUserName"      ,0x98  ,16 ,CHARACTER_MODE   ,NULL},//*
    {"MgntUserPasswrod"  ,0xA8  ,8  ,CHARACTER_MODE   ,NULL},//*
    {"SSID_1_Name"       ,0xB0  ,16 ,CHARACTER_MODE   ,NULL},
    {"SSID_1_Password"   ,0xC0  ,8  ,CHARACTER_MODE   ,NULL},
    {"SSID_2_Name"       ,0xC8  ,16 ,CHARACTER_MODE   ,NULL},
    {"SSID_2_Password"   ,0xD8  ,8  ,CHARACTER_MODE   ,NULL},
    {"OperatorID"        ,0xE0  ,4  ,CHARACTER_MODE   ,NULL},
    {"SLID"              ,0xE4  ,16 ,HEX_MODE         ,NULL},
    {"CountryID"         ,0xF4  ,2  ,CHARACTER_MODE   ,NULL},
    {"GroupID"           ,0xF6  ,4  ,HEX_MODE         ,NULL},//unknown
    {"Spare5"            ,0xFA  ,2  ,HEX_MODE         ,NULL},
    {"Checksum1"         ,0xFC  ,2  ,HEX_MODE         ,NULL},
    {"Spare6"            ,0xFE  ,2  ,HEX_MODE         ,NULL},
    {"RollbackFlag"      ,0x0   ,2  ,HEX_MODE         ,NULL},//0x57 i2c-device
};

int get_e2prom_data_map_length(void){
    return (sizeof(nokia_ri)/sizeof(E2PROM_DATA_MAP));
}

void print_all_support_parm(void){
    int e2prom_data_len = get_e2prom_data_map_length();
    int i = 0;
    printf("Support parm:\n");
    for(i = 0 ; i < e2prom_data_len ; i++){
        printf("%16s\t",nokia_ri[i].parm);
        if( (i+1)%4==0 || i==(e2prom_data_len-1) )
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
void set_MACAddress_help(void){
    fprintf(stderr,"MACAddress is not valid\n"
        "Please match the following format: 30:31:32:33:34:35\n");
    exit(1);
}

int parse_mac_addr(char *str){
    int ret = RET_FAILED;
    int i = 0;
    while( *str && i < MAC_STR_ADDRESS_LEN){
       if( (i%3)==2 ){
           if( *str != ':')
               return RET_FAILED;
       }
       else{
           if((*str >= '0') && (*str <= '9'))      ret = RET_SUCCESS;
           else if((*str >= 'a') && (*str <= 'f')) ret = RET_SUCCESS;
           else if((*str >= 'A') && (*str <= 'F')) ret = RET_SUCCESS;
           else{
               return RET_FAILED;
           }
       }
       str++;
       i++;
    }
    return ret;
}

void set_MACAddress(char *data,char *block,int *len){
   int i = 0;
   unsigned char mac[MAC_ADDRESS_LEN] = {0};

   if( data==NULL || strlen(data)!=MAC_STR_ADDRESS_LEN ){
       set_MACAddress_help();
   }
   else{
       if(parse_mac_addr(data) == RET_SUCCESS){
           sscanf(data,"%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",&mac[0],&mac[1],&mac[2],&mac[3],&mac[4],&mac[5]);
           for(i = 0 ; i < MAC_ADDRESS_LEN ; i++){
               block[i] = mac[i];
           }
           *len = MAC_ADDRESS_LEN;
       }
       else
           set_MACAddress_help();
   }
}

void set_ProgDate_help(void){
    fprintf(stderr,"ProgDate is not valid\n"
        "Please match the following format: 160101\n");
    exit(1);
}

int parse_prog_date(char *str){
    int ret = RET_FAILED;
    while( *str ){
        if((*str >= '0') && (*str <= '9'))    ret = RET_SUCCESS;
        else
            return RET_FAILED;
        str++;
    }
    return ret;
}

void set_ProgDate(char *data,char *block,int *len){
    int i = 0;
    if( data==NULL || strlen(data)!=PROG_DATE_LEN ){
        set_ProgDate_help();
    }
    else{
        if(parse_prog_date(data) == RET_SUCCESS){
            for( i = 0 ; i < PROG_DATE_LEN ; i++ ){
                block[i] = data[i];
            }
            *len = PROG_DATE_LEN;
        }
        else
            set_ProgDate_help();
    }
}

void set_FactoryCode_help(void){
    fprintf(stderr,"FactoryCode is not valid\n"
        "Please match the following format: 11\n");
    exit(1);
}

int parse_factorycode(char *str){
    int ret = RET_FAILED;
    while( *str ){
        if((*str >= '0') && (*str <= '9'))    ret = RET_SUCCESS;
        else
            return RET_FAILED;
        str++;
    }
    return ret;
}

void set_FactoryCode(char *data,char *block,int *len){
    int i = 0;
    if( data==NULL || strlen(data)!=FACTORY_CODE_LEN ){
        set_FactoryCode_help();
    }
    else{
        if(parse_factorycode(data) == RET_SUCCESS){
            for( i = 0 ; i < FACTORY_CODE_LEN ; i++ ){
                block[i] = data[i];
            }
            *len = FACTORY_CODE_LEN;
        }
        else
            set_FactoryCode_help();
    }
}

void set_ICS_help(void){
    fprintf(stderr,"ICS is not valid\n"
        "Please match the following format: 01\n");
    exit(1);
}

int parse_ics(char *str){
    int ret = RET_FAILED;
    while( *str ){
        if((*str >= '0') && (*str <= '9'))    ret = RET_SUCCESS;
        else
            return RET_FAILED;
        str++;
    }
    return ret;
}

void set_ICS(char *data,char *block,int *len){
    int i = 0;
    if( data==NULL || strlen(data)!=ICS_LEN ){
        set_ICS_help();
    }
    else{
        if(parse_ics(data) == RET_SUCCESS){
            for( i = 0 ; i < ICS_LEN ; i++ ){
                block[i] = data[i];
            }
            *len = ICS_LEN;
        }
        else
            set_ICS_help();
    }
}

void set_HardwareVersion_help(void){
    fprintf(stderr,"HardwareVersion is not valid\n"
        "Please match the following format: 3FE47122AAAA\n");
    exit(1);
}

int parse_hardware_version(char *str){
    int ret = RET_FAILED;
    while( *str ){
       if((*str >= '0') && (*str <= '9'))      ret = RET_SUCCESS;
       else if((*str >= 'a') && (*str <= 'f')) ret = RET_SUCCESS;
       else if((*str >= 'A') && (*str <= 'F')) ret = RET_SUCCESS;
       else{
           return RET_FAILED;
       }
       str++;
    }
    return ret;
}

void set_HardwareVersion(char *data,char *block,int *len){
    int i = 0;
    if( data==NULL || strlen(data)!=HARDWARE_VERSION_LEN ){
        set_HardwareVersion_help();
    }
    else{
        if(parse_hardware_version(data) == RET_SUCCESS){
            for( i = 0 ; i < HARDWARE_VERSION_LEN ; i++ ){
                block[i] = data[i];
            }
            *len = HARDWARE_VERSION_LEN;
        }
        else
            set_HardwareVersion_help();
    }
}

void set_YPSerialNum_help(void){
    fprintf(stderr,"YPSerialNum is not valid\n"
        "Please match the following format: ALCLFA68472B (16 chars max)\n");
    exit(1);
}

int parse_yp_serialnum(char *str){
    int ret = RET_FAILED;
	 printf("*str is 0x%x = %c\n", *str, *str);
    while( *str ){
       if((*str >= '0') && (*str <= '9'))      ret = RET_SUCCESS;
       else if((*str >= 'a') && (*str <= 'f')) ret = RET_SUCCESS;
       else if((*str >= 'A') && (*str <= 'F')) ret = RET_SUCCESS;
       else
           return RET_FAILED;

       str++;
    }
    return ret;
}

void set_YPSerialNum(char *data,char *block,int *len){
    int i = 0;
    if( strlen(data) > YPSERIALNUM_LEN )
        set_YPSerialNum_help();

    else{
        if(strlen(data) < YPSERIALNUM_LEN){
            for( i = 0 ; i < (YPSERIALNUM_LEN-strlen(data)) ; i++ )
                    block[i] = ' ';
        }
        for( i ; i < YPSERIALNUM_LEN ; i++ )
                block[i] = data[i-(YPSERIALNUM_LEN-strlen(data))];

        *len = YPSERIALNUM_LEN;
    }
}
//#endif //ODM_GEMTEK
