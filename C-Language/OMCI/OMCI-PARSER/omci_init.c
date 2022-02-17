#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "omci.h"

const char *omciParser_mode_name[]={
    "Normal Mode",
    "Filter Mode",
    "MeReplay Mode"
};

const omci_me_modules_t omci_me_modules_list[]={
    {"OLT-G",ME_9_12_2,me_9_12_2_parser},
    {"Extended VLAN tagging operation configuration data",ME_9_3_13,me_9_3_13_parser},
    {"Downstream VLAN Tag Egress Behavior Supplemental",ME_65305,me_65305_parser}
};
const int omci_me_modules_list_size = (sizeof(omci_me_modules_list)/sizeof(omci_me_modules_list[0]));

const omci_action_t omci_action_list[]={
    {1,0,4,OLT_CREATE},
    {1,0,8,OLT_SET},
    {0,1,8,ONU_SET_REP}
};
const int omci_action_list_size = (sizeof(omci_action_list)/sizeof(omci_action_list[0]));

void dbg_omci_me_modules_list(void)
{
#ifdef DBG_OMCI_ME_MODULES_LIST
    int i = 0;
    DEBUG_COLOR(LIGHT_CYAN,"OMCI ME Support Modules List : ");
    for(i=0;i<omci_me_modules_list_size;i++){
       DEBUG_COLOR(LIGHT_CYAN,"ME Name : %50s ; ME ID : %3u",omci_me_modules_list[i].meName,omci_me_modules_list[i].meId);
    }
    printf("\n");
#endif //DBG_OMCI_ME_MODULES_LIST
}
void dbg_module_info(omci_me_modules_t *module)
{
    if(module == NULL){
        ERROR("ME module is NULL");
        return ;
    }

    DEBUG_COLOR(LIGHT_CYAN,"ME Name : %50s ; ME ID : %3u",module->meName,module->meId);
}

