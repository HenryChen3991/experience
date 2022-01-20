#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "omci.h"

//main
void parser_omci_msg(char *raw_omci,int index,int mode,int filter_me)
{
    unsigned char omci_data[OMCI_USEFUL_MSG_LEN]={0};
    const omci_me_modules_t *module = NULL;
    int i = 0;
    cmsUtl_strToLower(raw_omci);
    DEBUG("OMCI MSG [%03d] : %s",index+1,raw_omci);
    convert2data(omci_data,raw_omci);

    omci_t *omci = (omci_t*)omci_data;

    if(filter_me != htons(omci->meid)){
        return ;
    }

    for(i=0;i<omci_me_modules_list_size;i++){
        if(omci_me_modules_list[i].meId == htons(omci->meid)){
            module = &omci_me_modules_list[i];
            break;
        }
    }

    if(module!=NULL){
        //dbg_module_info(module);
        if(module->parser){
            module->parser(omci,module->meName);
        }
    }
    else{
        NOTICE("Not support OMCI msg index %03d",index);
    }  
}

Ret replay_omci_msg(char *raw_omci,int index,FILE *outfp)
{
    unsigned char omci_data[OMCI_USEFUL_MSG_LEN]={0};
    const omci_me_modules_t *module = NULL;
    cmsUtl_strToLower(raw_omci);
    
    convert2data(omci_data,raw_omci);

    omci_t *omci = (omci_t*)omci_data;

    //dbg_omci_packet_format(omci);
    //printf("\n");

    if( omci->msgType == 0x4f || omci->msgType == 0x4d || omci->msgType == 0x44 || omci->msgType == 0x48){
        NOTICE("OMCI MSG [%03d] : %s",index+1,raw_omci);
        fputs(raw_omci,outfp);
        fputs("\n",outfp);
        return MATCH_MEREPLAY_RULES;
    }
    else
        DEBUG("OMCI MSG [%03d] : %s",index+1,raw_omci);

    return NOT_MATCH_MEREPLAY_RULES;
}

