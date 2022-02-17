#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "omci.h"

#define ME_65305_ATTR_NUM (2)

void me_65305_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_65305_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);

void me_65305_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{

}
void me_65305_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{

}

static omci_attr_handler_t omci_attr_handler_list[]={
    {0  ,"Managed entity ID"                 ,2  ,NULL},
    {1  ,"UNI Default TxMode Tag Behavior"   ,1  ,me_65305_attr_1_handler},
    {2  ,"VLAN TxMode Tag Behavior Table"    ,6  ,me_65305_attr_2_handler}
};

void me_65305_parser(omci_t *omci,const unsigned char *meName)
{
    int i = 0;
    msg_type_detail_t msg_type_detail;
    const omci_action_t *omci_action = NULL;
    unsigned char *ptr = NULL;
    dbg_omci_packet_format(omci);
    msg_type_detail.c = omci->msgType;
    if( (omci_action = matching_omci_action_list(msg_type_detail)) != NULL){
        DEBUG_COLOR(LIGHT_CYAN,"%s - %s",omci_action->name,meName);


        //Case: OLT_SET
        if(cmsUtl_strcmp(omci_action->name, OLT_SET)==0){
            ptr = (unsigned char*)omci->msgContect;
            //convert two byte to short - ex
            unsigned short attrMask = (omci->msgContect[0] << 8) | omci->msgContect[1];
            ptr += ATTR_MASK_SIZE_BYTE;

#ifdef DBG_ATTRMASK
            NW_DEBUG_COLOR(LIGHT_CYAN,"AttrMask = %0x ; Covert to binary : ",attrMask);
            print_short_type_bin(attrMask);
#endif //DBG_ATTRMASK

            for(i=1;i<=ATTR_MASK_LEN;i++){
                if(GET_ATTR_INDEX(attrMask,i)){
                    if(omci_attr_handler_list[i].handler != NULL)
                        omci_attr_handler_list[i].handler(ptr,omci_attr_handler_list[i].name,omci_attr_handler_list[i].size,i);
                        ptr += omci_attr_handler_list[i].size;
                }
            }
        }


        //Case: OLT_CREATE
        if(cmsUtl_strcmp(omci_action->name, OLT_CREATE)==0){
            
        }

        printf("\n");
    }
    
}

