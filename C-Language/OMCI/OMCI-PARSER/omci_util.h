#ifndef OMCI_UTIL_H
#define OMCI_UTIL_H

/**
 * Define DBG_OMCI_FLAG Cnntrol
 */
//#define DBG_OMCI 1
//#define DBG_ATTRMASK 1

void parser_omci_msg( char *raw_omci,int index);
void dbg_omci_packet_format(omci_t *omci);
void print_omci_msg_hex(unsigned char *omci);
void convert2data(unsigned char *omci,char *raw_omci);
const omci_action_t * matching_omci_action_list(msg_type_detail_t msg_type_detail);

#define GET_ATTR_INDEX(mask,index) \
({ \
        int bit = index -1; \
        if (bit < 0) ERROR("index must start from 1\n");\
        mask >> (15-bit) & 0x01; \
})

void print_attr_name(const unsigned char *attr_name,int index);
void print_ocmi_data_and_hex(char *color,const unsigned char *name,const unsigned char *str,int size,int index);

#endif //OMCI_UTIL_H


