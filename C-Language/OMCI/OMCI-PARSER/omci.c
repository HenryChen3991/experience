#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "omci.h"

//main
void parser_omci_msg(char *raw_omci,int index)
{
    unsigned char omci_data[OMCI_USEFUL_MSG_LEN]={0};
    const omci_me_modules_t *module = NULL;
    int i = 0;
    cmsUtl_strToLower(raw_omci);
    DEBUG("OMCI MSG [%03d] : %s",index,raw_omci);
    convert2data(omci_data,raw_omci);

    omci_t *omci = (omci_t*)omci_data;

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

