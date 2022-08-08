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
void parser_dscpToPbitMapping(unsigned char* dscpToPbitMapping);

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
    unsigned char dscpToPbitMapping[24];
    memcpy(dscpToPbitMapping,content,size);
    print_ocmi_data_and_hex(LIGHT_CYAN,attr_name,dscpToPbitMapping,size,index);
    parser_dscpToPbitMapping(dscpToPbitMapping);
}

/**
 * Step 1 : each three byte combine to a integer data
 * src[0]       = 0x      92
 * src[1]       = 0x      49
 * src[2]       = 0x      24
 * groups[i]    = 0x00924924
 *
 * groups[i]    = 00000000100100100100100100100100
 * Step 2 : data & 0x7 to get three bit data and then shift 3 bit
 */

/** Size of intermediate array for DSCP conversion */
#define DSCP_CONVERSION_GROUPS 8
#define DSCP_MAX 64

void parser_dscpToPbitMapping(unsigned char* src){
    DEBUG_COLOR(LIGHT_GREEN,"Enter parser_dscpToPbitMapping");
    unsigned int mapping[64]={0};
    int i, j, k;
    unsigned int groups[DSCP_CONVERSION_GROUPS]={0};
    /* Group bits into 8 x 24 bit groups */
	j = 0;
	for (i = 0; i < DSCP_CONVERSION_GROUPS; ++i) {
        //DEBUG_COLOR(LIGHT_GREEN,"src[%d] = 0x%8x",j,src[j]);

		groups[i] = (unsigned int)src[j++] << 16;

        //DEBUG_COLOR(LIGHT_GREEN,"src[%d] = 0x%8x",j,src[j]);

		groups[i] |= (unsigned int)src[j++] << 8;

        //DEBUG_COLOR(LIGHT_GREEN,"src[%d] = 0x%8x",j,src[j]);

		groups[i] |= src[j++];

        //DEBUG_COLOR(LIGHT_GREEN,"src[%d] = 0x%08x",i,groups[i]);
        //print_int_type_bin(groups[i]);
	}

    /* Convert prepared groups into 64 dscp */
	k = 0;
	for (i = 0; i < DSCP_CONVERSION_GROUPS; ++i) {
		for (j = k + 7; j >= k; --j) {
			mapping[j] = groups[i] & 0x7;
			groups[i] >>= 3;
		}
		k += 8;
	}

    //print dscpToPbitMapping table
    SET_COLOR(LIGHT_GREEN);
    printf("%20s:%4d|\n",__FILE__,__LINE__);
    for(i = 0;i<DSCP_MAX;i+=8){
        printf("\t\t\t   map[%02d]:%d  map[%02d]:%d  map[%02d]:%d  map[%02d]:%d  map[%02d]:%d  map[%02d]:%d  map[%02d]:%d  map[%02d]:%d\n"
            ,i,mapping[i],i+1,mapping[i+1]
            ,i+2,mapping[i+2],i+3,mapping[i+3]
            ,i+4,mapping[i+4],i+5,mapping[i+5]
            ,i+6,mapping[i+6],i+7,mapping[i+7]);
    }
    SET_COLOR(NONECOLOR);

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
    unsigned int filterOuterPrio = m->filterOuterPrio;
    unsigned int filterOuterVid = m->filterOuterVid;
    unsigned int filterInnerPrio = m->filterInnerPrio;
    unsigned int filterInnerVid = m->filterInnerVid;
    unsigned int treatmentTagsRemove = m->treatmentTagsRemove;
    unsigned int treatmentOuterPrio = m->treatmentOuterPrio;
    unsigned int treatmentOuterVid = m->treatmentOuterVid;
    unsigned int treatmentInnerPrio = m->treatmentInnerPrio;
    unsigned int treatmentInnerVid = m->treatmentInnerVid;


    printf("%s%20s:%4d| ",LIGHT_CYAN,__FILE__,__LINE__);

    if ( filterOuterPrio == THIS_ENRTY_IS_NOT_A_DOUBLE_TAG_RULE &&
         filterInnerPrio == THIS_ENTRY_IS_A_NO_TAG_RULE ) //untag
    {
        printf("Untag         frames : ");
        printf("F -> ");

        if(treatmentTagsRemove == DROP_THE_FRAME){
            printf("Drop");
        }
        else{
            if(treatmentOuterPrio != DO_NOT_ADD_AN_OUTER_TAG && treatmentOuterPrio >= 0 && treatmentOuterPrio <= 7)
                printf("%d/%d-F",treatmentOuterVid,treatmentOuterPrio);
            else if( treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                printf("%d-F (derive Pbits from the DSCP field",treatmentOuterVid);

            if(treatmentInnerPrio != DO_NOT_ADD_AN_INNER_TAG && treatmentInnerPrio >= 0 && treatmentInnerPrio <= 7)
                printf("%d/%d-F",treatmentInnerVid,treatmentInnerPrio);
            else if(treatmentInnerPrio == ADD_AN_INNER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                printf("%d-F (derive Pbits from the DSCP field",treatmentInnerVid);
        }
    }
    else if ( filterOuterPrio == 15 &&  filterOuterVid == 4096 && (filterInnerPrio != 15 ||  filterInnerVid != 4096)) //single tag
    {
        printf("Single tagged frames : ");

        if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
            printf("%d/%d-F -> ",filterInnerVid,filterInnerPrio);
        else
            printf("%d-F -> ",filterInnerVid);

        if(treatmentTagsRemove == DROP_THE_FRAME){
            printf("Drop");
        }
        else if(treatmentTagsRemove == REMOVE_ZERO_TAG){
            if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                printf("%d/%d-F",filterInnerVid,filterInnerPrio);
            else
                 printf("%d-F",filterInnerVid);

            if(treatmentOuterPrio != DO_NOT_ADD_AN_OUTER_TAG && treatmentOuterPrio >= 0 && treatmentOuterPrio <= 7)
                printf(" + %d/%d-F",treatmentOuterVid,treatmentOuterPrio);
            else if(treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_COPY_OUTER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME)
                printf(" + %d/%d-F",treatmentOuterVid,filterInnerPrio);
            else if( treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                printf(" + %d-F (derive Pbits from the DSCP field",treatmentOuterVid);

            if(treatmentInnerPrio != DO_NOT_ADD_AN_INNER_TAG && treatmentInnerPrio >= 0 && treatmentInnerPrio <= 7)
                printf(" + %d/%d-F",treatmentInnerVid,treatmentInnerPrio);
            else if (treatmentInnerPrio == ADD_AN_INNER_TAG_AND_COPY_INNER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME)
                printf(" + %d/%d-F",treatmentInnerVid,filterInnerPrio);
            else if(treatmentInnerPrio == ADD_AN_INNER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                printf(" + %d-F (derive Pbits from the DSCP field",treatmentInnerVid);
        }
        else if(treatmentTagsRemove == REMOVE_ONE_TAG){
            if(treatmentOuterPrio == DO_NOT_ADD_AN_OUTER_TAG && treatmentInnerPrio == DO_NOT_ADD_AN_INNER_TAG)
                printf("F");
            else{
                //ADD Outer vlan
                if(treatmentOuterPrio != DO_NOT_ADD_AN_OUTER_TAG){
                    if(treatmentOuterPrio >= 0 && treatmentOuterPrio <= 7){
                        printf("%d/%d-F",treatmentOuterVid,treatmentOuterPrio);
                    }
                    else if(treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_COPY_OUTER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME){
                        if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                            printf("%d/%d-F",treatmentOuterVid,filterInnerPrio);
                        else
                            printf("%d-F",treatmentOuterVid);
                        }
                    else if( treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE){
                        printf("%d-F (derive Pbits from the DSCP field",treatmentOuterVid);
                    }
                    printf(" + ");
                }
                //ADD Inner vlan
                if(treatmentInnerPrio != DO_NOT_ADD_AN_INNER_TAG){
                    if(treatmentInnerPrio >= 0 && treatmentInnerPrio <= 7)
                        printf("%d/%d-F",treatmentInnerVid,treatmentInnerPrio);
                    else if (treatmentInnerPrio == ADD_AN_INNER_TAG_AND_COPY_INNER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME){
                        if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                            printf("%d/%d-F",treatmentInnerVid,filterInnerPrio);
                        else
                            printf("%d-F",treatmentInnerVid);
                    }
                    else if(treatmentInnerPrio == ADD_AN_INNER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                        printf("%d-F (derive Pbits from the DSCP field",treatmentInnerVid);
                }
            }
        }
    }
    else //Double tag
    {
        printf("Double tagged frames : ");

        if(filterOuterPrio >= 0 && filterOuterPrio <= 7)
            printf("%d/%d-F + ",filterOuterVid,filterOuterPrio);
        else
            printf("%d-F + ",filterOuterVid);

        if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
            printf("%d/%d-F -> ",filterInnerVid,filterInnerPrio);
        else
            printf("%d-F -> ",filterInnerVid);

        //B part
        if(treatmentTagsRemove == DROP_THE_FRAME){
            printf("Drop");
        }
        
        //ADD Outer vlan
        if(treatmentOuterPrio != DO_NOT_ADD_AN_OUTER_TAG){
            if(treatmentOuterPrio >= 0 && treatmentOuterPrio <= 7){
                printf("%d/%d-F",treatmentOuterVid,treatmentOuterPrio);
            }
            else if(treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_COPY_OUTER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME){
                if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                    printf("%d/%d-F",treatmentOuterVid,filterInnerPrio);
                else
                    printf("%d-F",treatmentOuterVid);
            }
            else if(treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_COPY_OUTER_PRIO_FROM_OUTER_PRIO_OF_RECEIVED_FRAME){
                if(filterOuterPrio >= 0 && filterOuterPrio <= 7)
                    printf("%d/%d-F",treatmentOuterVid,filterOuterPrio);
                else
                    printf("%d-F",treatmentOuterVid);
            }
            else if( treatmentOuterPrio == ADD_AN_OUTER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE){
                printf("%d-F (derive Pbits from the DSCP field",treatmentOuterVid);
            }
            printf(" + ");
        }
        //ADD Inner vlan
        if(treatmentInnerPrio != DO_NOT_ADD_AN_INNER_TAG){
            if(treatmentInnerPrio >= 0 && treatmentInnerPrio <= 7)
                printf("%d/%d-F",treatmentInnerVid,treatmentInnerPrio);
            else if (treatmentInnerPrio == ADD_AN_INNER_TAG_AND_COPY_INNER_PRIO_FROM_INNER_PRIO_OF_RECEIVED_FRAME){
                if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                    printf("%d/%d-F",treatmentInnerVid,filterInnerPrio);
                else
                    printf("%d-F",treatmentInnerVid);
            }
            else if (treatmentInnerPrio == ADD_AN_INNER_TAG_AND_COPY_INNER_PRIO_FROM_OUTER_PRIO_OF_RECEIVED_FRAME ){
                if(filterOuterPrio >= 0 && filterOuterPrio <= 7)
                    printf("%d/%d-F",treatmentInnerVid,filterOuterPrio);
                else
                    printf("%d-F",treatmentInnerVid);
            }
            else if(treatmentInnerPrio == ADD_AN_INNER_TAG_AND_DERIVE_PBIT_FROM_DSCP_TABLE)
                printf("%d-F (derive Pbits from the DSCP field",treatmentInnerVid);
            
            printf(" + ");
        }

        if(treatmentTagsRemove == REMOVE_ZERO_TAG){
            if(filterOuterPrio >= 0 && filterOuterPrio <= 7)
                printf("%d/%d-F + ",filterOuterVid,filterOuterPrio);
            else
                printf("%d-F + ",filterOuterVid);

            if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                printf("%d/%d-F",filterInnerVid,filterInnerPrio);
            else
                printf("%d-F",filterInnerVid);
        }
        else if(treatmentTagsRemove == REMOVE_ONE_TAG){
            if(filterInnerPrio >= 0 && filterInnerPrio <= 7)
                printf("%d/%d-F",filterInnerVid,filterInnerPrio);
            else
                printf("%d-F",filterInnerVid);
        }
    }
    printf("%s\n",NONECOLOR);
}
#if 0
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
        printf(" -");
        outerf = 1;
    }

    if(m->filterInnerPrio != 15 && m->filterInnerVid != 4096){
        printf("%d",m->filterInnerVid);
        if(m->filterInnerPrio>=0 && m->filterInnerPrio<=7)
            printf("/%d",m->filterInnerPrio);
        printf(" -");
        innerf = 1;
    }
    else if(m->filterInnerPrio == 8 && m->filterInnerVid == 4096)
    {
        printf(" 4096");
    }

    if(innerf == 0 && outerf ==0)
        printf("F --> ");
    else
        printf(" F --> ");

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
#endif //0

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

