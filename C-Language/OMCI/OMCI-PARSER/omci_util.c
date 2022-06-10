#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ckl_debug.h"
#include "gtk_string.h"
#include "omci.h"

/*
 首先得到hexstring即16進制的字符串
 然後用sscanf將字符串解釋成2進制數組
 這裏"%2hhx" 2為width即一次最多讀入2個字符並且將它們解釋成16進制數存於val中

 二、占用空间不同
    1、％hhu：％hhu的占用空间为一个字节
    2、％hu：％hu的占用空间为两个字节
 三、数值范围不同
    1、％hhu：％hhu的数值范围为0到255［2^8－1］
    2、％hu：％hu的数值范围为为0到65535［2^16－1］
*/
void convert2data(unsigned char *omci,char *raw_omci){
    int i = 0;
    for(i=0;i<OMCI_USEFUL_MSG_LEN;i++){
        sscanf(raw_omci, "%2hhx", &omci[i]);
        raw_omci += 2;
    }
#ifdef DBG_OMCI    
    print_omci_msg_hex(omci);
#endif //DBG_OMCI
}

void print_omci_msg_hex(unsigned char *omci)
{
    int i = 0;
    printf("\t\t\t\t\t   ");
    for(i=0;i<OMCI_USEFUL_MSG_LEN;i++){
        printf("%02x",omci[i]);
    }
    printf("\n");
}
void dbg_omci_packet_format(omci_t *omci)
{
    int i = 0;
    DEBUG("OMCI TCID      : %04x",htons(omci->tcid));
    DEBUG("OMCI MSG TYPE  : %02x",omci->msgType);
    DEBUG("OMCI DEVICE ID : %02x",omci->deviceId);
    DEBUG("OMCI ME ID     : %03d",htons(omci->meid));
    DEBUG("OMCI INSTANCE  : %0d",htons(omci->instance));
    printf("%20s:%4d| " "OMCI CONTECT   : ",__FILE__,__LINE__);
    for(i=0;i<OMCI_MSG_CONTENT_LEN;i++){
        printf("%02x",omci->msgContect[i]);
    }
    printf("\n");
}

const omci_action_t *matching_omci_action_list(msg_type_detail_t msg_type_detail)
{
    int i = 0;
    for(i=0;i<omci_action_list_size;i++){
        if(msg_type_detail.ak == omci_action_list[i].ak && 
           msg_type_detail.ar == omci_action_list[i].ar &&
           msg_type_detail.mt == omci_action_list[i].mt )
        {
            return &omci_action_list[i];
        }
    }
    return NULL;
}

void print_attr_name(const unsigned char *attr_name,int index)
{
    DEBUG_COLOR(LIGHT_CYAN,"Enter Attr[%03d] %s handler",index,attr_name);
}

void print_ocmi_data_and_hex(char *color,const unsigned char *name,const unsigned char *str,int size,int index)
{
    int i = 0;
    //printf("%s%20s:%4d Attr[%03d] %s : %s (",color,__FILE__,__LINE__,index,name,str);
    printf("%s%20s:%4d| Attr[%03d] %s : (",color,__FILE__,__LINE__,index,name);
    for (i=0; i<size; i++)
    {
       printf("%02x",str[i]);
    }
    /* //printf char
    for (i=0; i<size; i++)
    {
       if(str[i]>=0x21 && str[i]<=0x7E)
           printf("%c",str[i]);
    }
    */
    printf("%s)\n",NONECOLOR);
}

