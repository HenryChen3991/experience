#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "omci.h"


void me_9_3_13_parser(omci_t *omci,const unsigned char *meName)
{
    msg_type_detail_t msg_type_detail;
    const omci_action_t *omci_action = NULL;
    dbg_omci_packet_format(omci);
    msg_type_detail.c = omci->msgType;
    if( (omci_action = matching_omci_action_list(msg_type_detail)) != NULL){
        DEBUG_COLOR(LIGHT_CYAN,"%s - %s",omci_action->name,meName);
    }
    printf("\n");
}

