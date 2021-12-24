#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ckl_debug.h"
static void analyze_parameter(int argc, char *argv[]);

#define APP "omci2pcap"

void usage(void){
    DEBUG("./%s [input] [output]",APP);
    DEBUG("     ex: ./%s omci.txt omci.pcap",APP);
}
static void analyze_parameter(int argc, char *argv[])
{
    DEBUG_COLOR(LIGHT_RED, "argc = %d",argc);
    if(argc < 2 /*|| access(argv[1],R_OK) != CMSRET_SUCCESS*/){
        usage();
        exit(0);
    }
    if( access(argv[1],R_OK)!= CMSRET_SUCCESS ){
        
    }
}

int main(int argc,char **argv)
{
    char *file = NULL;
    analyze_parameter(argc,argv);
    return 0;
}
