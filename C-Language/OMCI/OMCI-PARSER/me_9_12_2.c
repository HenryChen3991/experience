#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <inttypes.h>

#include "omci.h"

/**
 * Name          : OLT-G
 * Description   : This optional managed entity identifies the OLT to which an ONT is connected.
                   This ME provides a way for the ONT to configure itself for operability with a particular OLT. 
 * ME ID         : 131
 * Relationships : The single instance of this ME is associated with the ONU ME. 
 * Parser        : me_9_12_2_parser
 */
#define ME_131_ATTR_NUM 4

void me_131_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_131_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_131_attr_3_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_131_attr_4_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);

static omci_attr_handler_t omci_attr_handler_list[]={
    {0  ,"Managed entity ID"        ,2  ,NULL},
    {1  ,"OLT vendor ID"            ,4  ,me_131_attr_1_handler},
    {2  ,"Equipment ID"             ,20 ,me_131_attr_2_handler},
    {3  ,"Version"                  ,14 ,me_131_attr_3_handler},
    {4  ,"Time of day information"  ,14 ,me_131_attr_4_handler}
};

void me_131_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    unsigned char olt_vendor_id[4]={0};
    memcpy(olt_vendor_id,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,olt_vendor_id,size,index);
}
void me_131_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    unsigned char equipmentId[20]={0};
    memcpy(equipmentId,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,equipmentId,size,index);
}
void me_131_attr_3_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    unsigned char version[14]={0};
    memcpy(version,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,version,size,index);
}
void me_131_attr_4_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    unsigned char timeOfDayInfo[14]={0};
    memcpy(timeOfDayInfo,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,timeOfDayInfo,size,index);
}

void me_9_12_2_parser(omci_t *omci,const unsigned char *meName)
{
    int i = 0;
    msg_type_detail_t msg_type_detail;
    const omci_action_t *omci_action = NULL;
    unsigned char *ptr = NULL;
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


        //Case

        printf("\n");
    }
}

