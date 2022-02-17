#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "omci.h"

#define ME_171_ATTR_NUM (8)

void me_171_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_3_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_4_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_5_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_6_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_7_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
void me_171_attr_8_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);

void parser_received_frame_VLAN_tagging_operation_table(unsigned char *receivedFrameVLANTaggingOperationTable,int size);
void dbg_VLANTaggingOperationTableMapping(VLANTaggingOperationTableMapping_t m);
void parser_vlan_tagging_operation(VLANTaggingOperationTableMapping_t *m);
static omci_attr_handler_t omci_attr_handler_list[]={
    {0  ,"Managed entity ID"                                             ,2  ,NULL},
    {1  ,"Association type"                                              ,1  ,me_171_attr_1_handler},
    {2  ,"Received  frame  VLAN  tagging  operation  table  max  size"   ,2  ,me_171_attr_2_handler},
    {3  ,"Input TPID"                                                    ,2  ,me_171_attr_3_handler},
    {4  ,"Output  TPID"                                                  ,2  ,me_171_attr_4_handler},
    {5  ,"Downstream mode"                                               ,1  ,me_171_attr_5_handler},
    {6  ,"Received frame VLAN tagging operation table"                   ,16 ,me_171_attr_6_handler},
    {7  ,"Associated ME pointer"                                         ,2  ,me_171_attr_7_handler},
    {8  ,"DSCP to P-bit mapping"                                         ,24 ,me_171_attr_8_handler}
};

void me_171_attr_1_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    print_attr_name(attr_name,index);
}
void me_171_attr_2_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    print_attr_name(attr_name,index);
}
void me_171_attr_3_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    print_attr_name(attr_name,index);
}
void me_171_attr_4_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
   print_attr_name(attr_name,index);
}
void me_171_attr_5_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    print_attr_name(attr_name,index);
}
void me_171_attr_6_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    unsigned char receivedFrameVLANTaggingOperationTable[16]={0};
    memcpy(receivedFrameVLANTaggingOperationTable,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,receivedFrameVLANTaggingOperationTable,size,index);
    parser_received_frame_VLAN_tagging_operation_table(receivedFrameVLANTaggingOperationTable,size);
}
void me_171_attr_7_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
    print_attr_name(attr_name,index);
}

void me_171_attr_8_handler(unsigned char *content,unsigned char *attr_name,unsigned int size,int index)
{
   print_attr_name(attr_name,index);
}

void parser_received_frame_VLAN_tagging_operation_table(unsigned char *receivedFrameVLANTaggingOperationTable,int size)
{
#if 0
    unsigned char *data = receivedFrameVLANTaggingOperationTable;
    unsigned int d1 = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
    unsigned int d2 = data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7];
    unsigned int d3 = data[8] << 24 | data[9] << 16 | data[10] << 8 | data[11];
    unsigned int d4 = data[12] << 24 | data[13] << 16 | data[14] << 8 | data[15];
    printf("%08X\n",d1);
    printf("%08X\n",d2);
    printf("%08X\n",d3);
    printf("%08X\n",d4);
#endif //0

    unsigned int *word = NULL;
    word = (unsigned int *)receivedFrameVLANTaggingOperationTable;

    VLANTaggingOperationTableMapping_t m;
    m.word[0] =  ntohl(*word++);
    m.word[1] =  ntohl(*word++);
    m.word[2] =  ntohl(*word++);
    m.word[3] =  ntohl(*word++);
    dbg_VLANTaggingOperationTableMapping(m);
    parser_vlan_tagging_operation(&m);
}

void dbg_VLANTaggingOperationTableMapping(VLANTaggingOperationTableMapping_t m){
    DEBUG_COLOR(LIGHT_GREEN,"word: %08x | %08x | %08x | %08x",m.word[0],m.word[1],m.word[2],m.word[3]);
    DEBUG_COLOR(LIGHT_GREEN,"word[1] filterOuterPrio      - %08x - %d",m.filterOuterPrio,m.filterOuterPrio);
    DEBUG_COLOR(LIGHT_GREEN,"word[1] filterOuterVid       - %08x - %d",m.filterOuterVid,m.filterOuterVid);
    //DEBUG_COLOR(LIGHT_GREEN,"word[1] filterOuterTpidde    - %08x - %d",m.filterOuterTpidde,m.filterOuterTpidde);
    //DEBUG_COLOR(LIGHT_GREEN,"word[1] word1Padding         - %08x - %d",m.word1Padding,m.word1Padding);
    printf("\n");
    
    DEBUG_COLOR(LIGHT_GREEN,"word[2] filterInnerPrio      - %08x - %d",m.filterInnerPrio,m.filterInnerPrio);
    DEBUG_COLOR(LIGHT_GREEN,"word[2] filterInnerVid       - %08x - %d",m.filterInnerVid,m.filterInnerVid);
    DEBUG_COLOR(LIGHT_GREEN,"word[2] filterInnerTpidde    - %08x - %d",m.filterInnerTpidde,m.filterInnerTpidde);
    //DEBUG_COLOR(LIGHT_GREEN,"word[2] word2Padding         - %08x - %d",m.word2Padding,m.word2Padding);
    DEBUG_COLOR(LIGHT_GREEN,"word[2] filterEtherType      - %08x - %d",m.filterEtherType,m.filterEtherType);
    printf("\n");

    DEBUG_COLOR(LIGHT_GREEN,"word[3] treatmentTagsRemove  - %08x - %d",m.treatmentTagsRemove,m.treatmentTagsRemove);
    //DEBUG_COLOR(LIGHT_GREEN,"word[3] word3Padding         - %08x - %d",m.word3Padding,m.word3Padding);
    DEBUG_COLOR(LIGHT_GREEN,"word[3] treatmentOuterPrio   - %08x - %d",m.treatmentOuterPrio,m.treatmentOuterPrio);
    DEBUG_COLOR(LIGHT_GREEN,"word[3] treatmentOuterVid    - %08x - %d",m.treatmentOuterVid,m.treatmentOuterVid);
    //DEBUG_COLOR(LIGHT_GREEN,"word[3] treatmentOuterTpidde - %08x - %d",m.treatmentOuterTpidde,m.treatmentOuterTpidde);
    printf("\n");
    
    //DEBUG_COLOR(LIGHT_GREEN,"word[4] word4Padding         - %08x - %d",m.word4Padding,m.word4Padding);
    DEBUG_COLOR(LIGHT_GREEN,"word[4] treatmentInnerPrio   - %08x - %d",m.treatmentInnerPrio,m.treatmentInnerPrio);
    DEBUG_COLOR(LIGHT_GREEN,"word[4] treatmentInnerVid    - %08x - %d",m.treatmentInnerVid,m.treatmentInnerVid);
    //DEBUG_COLOR(LIGHT_GREEN,"word[4] treatmentInnerTpidde - %08x - %d",m.treatmentInnerTpidde,m.treatmentInnerTpidde);
}

void parser_vlan_tagging_operation(VLANTaggingOperationTableMapping_t *m)
{
    printf("%s%20s:%4d ===>  ",LIGHT_CYAN,__FILE__,__LINE__);
    int outerf = 0;
    int innerf = 0;
    //part 1
    if(m->treatmentTagsRemove == 3){
        printf("ignore\n");
        return;
    }

        //part 2
    if(m->filterOuterPrio != 15 && m->filterOuterVid != 4096){
        printf("%d",m->filterOuterVid);
        if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
            printf("/%d",m->filterOuterPrio);
        printf("-");
        outerf = 1;
    }

    if(m->filterInnerPrio != 15 && m->filterInnerVid != 4096){
        printf("%d",m->filterInnerVid);
        if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
            printf("/%d",m->filterInnerPrio);
        printf("-");
        innerf = 1;
    }
    printf("F --> ");

    //part 3
    if(m->treatmentTagsRemove == 0){
        if(m->treatmentOuterPrio != 15){

            if(m->treatmentOuterVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentOuterVid);
            }

            //pbit part
            if(m->treatmentOuterPrio>=0 && m->treatmentOuterPrio<=7)
                printf("/%d",m->treatmentOuterPrio);
            else if(m->treatmentOuterPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentOuterPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");
        }
        if(m->treatmentInnerPrio != 15){

            if(m->treatmentInnerVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentInnerVid);
            }

            //pbit part
            if(m->treatmentInnerPrio>=0 && m->treatmentInnerPrio<=7)
                printf("/%d",m->treatmentInnerPrio);
            else if(m->treatmentInnerPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentInnerPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");
        }
        if(outerf){
            printf("%d",m->filterOuterVid);
            if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                printf("/%d",m->filterOuterPrio);
            printf("-");
        }
        if(innerf){
            printf("%d",m->filterInnerVid);
            if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                printf("/%d",m->filterInnerPrio);
            printf("-");
        }
        printf("F");
    }
    else if(m->treatmentTagsRemove == 1){
        if(m->treatmentOuterPrio != 15){

            if(m->treatmentOuterVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentOuterVid);
            }

            //pbit part
            if(m->treatmentOuterPrio>=0 && m->treatmentOuterPrio<=7)
                printf("/%d",m->treatmentOuterPrio);
            else if(m->treatmentOuterPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentOuterPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");

            if(outerf && innerf){
                printf("%d",m->filterInnerVid);
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
                printf("-");
            }
        }

        if(m->treatmentInnerPrio != 15){

            if(m->treatmentInnerVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentInnerVid);
            }

            //pbit part
            if(m->treatmentInnerPrio>=0 && m->treatmentInnerPrio<=7)
                printf("/%d",m->treatmentInnerPrio);
            else if(m->treatmentInnerPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentInnerPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");

            if(outerf && innerf){
                printf("%d",m->filterInnerVid);
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
                printf("-");
            }
        }
        printf("F");
    }
    else if(m->treatmentTagsRemove == 2){
        if(m->treatmentOuterPrio != 15){

            if(m->treatmentOuterVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentOuterVid);
            }

            //pbit part
            if(m->treatmentOuterPrio>=0 && m->treatmentOuterPrio<=7)
                printf("/%d",m->treatmentOuterPrio);
            else if(m->treatmentOuterPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentOuterPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");
        }
        if(m->treatmentInnerPrio != 15){

            if(m->treatmentInnerVid == 4096){
                printf("%d",m->filterInnerVid);
            }
            else if(m->treatmentOuterVid == 4097){
                printf("%d",m->filterOuterVid);
            }
            else{
                printf("%d",m->treatmentInnerVid);
            }

            //pbit part
            if(m->treatmentInnerPrio>=0 && m->treatmentInnerPrio<=7)
                printf("/%d",m->treatmentInnerPrio);
            else if(m->treatmentInnerPrio == 8){
                if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
                    printf("/%d",m->filterInnerPrio);
            }
            else if(m->treatmentInnerPrio == 9){
                if(m->filterOuterPrio>=0 && m->filterOuterPrio<=7)
                    printf("/%d",m->filterOuterPrio);
            }
            printf("-");
        }
        printf("F");
    }

    
    printf("%s\n",NONECOLOR);
}

void me_9_3_13_parser(omci_t *omci,const unsigned char *meName)
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

