#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <inttypes.h>

#include "omci.h"

/**
 * Name        : OLT-G
 * Description : This optional managed entity identifies the OLT to which an ONT is connected.
                 This ME provides a way for the ONT to configure itself for operability with a particular OLT. 
 * ME ID       : 131
 * Parser      : me_9_12_2_parser
 */
#define ME_131_ATTR_NUM 4

void me_131_attr_1_handler(unsigned char *content,unsigned char *attr_name);
void me_131_attr_2_handler(unsigned char *content,unsigned char *attr_name);
void me_131_attr_3_handler(unsigned char *content,unsigned char *attr_name);
void me_131_attr_4_handler(unsigned char *content,unsigned char *attr_name);

omci_attr_handler_t omci_attr_handler_list[]={
    {0  ,"NULL"                     ,NULL},
    {1  ,"OLT vendor ID"            ,me_131_attr_1_handler},
    {2  ,"Equipment ID"             ,me_131_attr_2_handler},
    {3  ,"Version"                  ,me_131_attr_3_handler},
    {4  ,"Time of day information"  ,me_131_attr_4_handler}
};

void me_131_attr_1_handler(unsigned char *content,unsigned char *attr_name)
{
    print_attr_name(attr_name);
}
void me_131_attr_2_handler(unsigned char *content,unsigned char *attr_name)
{
    print_attr_name(attr_name);
}
void me_131_attr_3_handler(unsigned char *content,unsigned char *attr_name)
{
    print_attr_name(attr_name);
}
void me_131_attr_4_handler(unsigned char *content,unsigned char *attr_name)
{
    print_attr_name(attr_name);
}

void me_9_12_2_parser(omci_t *omci,const unsigned char *meName)
{
    int i = 0;
    msg_type_detail_t msg_type_detail;
    const omci_action_t *omci_action = NULL;
    dbg_omci_packet_format(omci);
    
    msg_type_detail.c = omci->msgType;
#if 0
    DEBUG("mt = %0d",msg_type_detail.mt);
    DEBUG("ak = %0d",msg_type_detail.ak);
    DEBUG("ar = %0d",msg_type_detail.ar);
    DEBUG("db = %0d",msg_type_detail.db);
#endif

    if( (omci_action = matching_omci_action_list(msg_type_detail)) != NULL){
        DEBUG_COLOR(LIGHT_CYAN,"%s - %s",omci_action->name,meName);

        //convert two byte to short - ex
        unsigned short attrMask = (omci->msgContect[0] << 8) + omci->msgContect[1];
        //sscanf(omci->msgContect, "%2hhu", &attrMask);
        NW_DEBUG_COLOR(LIGHT_CYAN,"AttrMask = %0x ; Covert to binary : ",attrMask);
        print_bin(attrMask);

        for(i=1;i<=ATTR_MASK_LEN;i++){
            if(GET_ATTR_INDEX(attrMask,i)){
                if(omci_attr_handler_list[i].handler != NULL)
                    omci_attr_handler_list[i].handler(omci->msgContect,omci_attr_handler_list[i].attr_name);
            }
        }
    }
    printf("\n");
}

