#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "omci.h"

static void analyze_parameter(int argc, char *argv[]);

#define APP "omciparser"
#define WIRESHARK_HEADER "00 90 d0 00 00 00 0a 76 ff 0c 8d 60 88 b5 "
#define OMCI_TRAILER "00 00 00 28 dc a2 62 5e"
#define ROW_FORMAT "00%d0 "





//Transcation ID 2 
//Messgae type 1
//Device ID 1
//Message ID 4
//Message content 32
#define OMCI_PACKET_LEN (40)

void usage(void){
    printf(LIGHT_RED"Invalid parpmeters\n\n"NONECOLOR);
    printf("Usage:\n\t \
        ./%s [input]\nex:\n\t \
        ./%s omci.txt\n",APP,APP);
}
static void analyze_parameter(int argc, char *argv[])
{
    if(argc < 2 || access(argv[1],R_OK) != CMSRET_SUCCESS){
        usage();
        exit(0);
    }
}

Ret omciparser(char *Infile){
    FILE *infp = NULL;
    Ret ret = CMSRET_SUCCESS;
    char buf[BUFLEN_256] = {0};
    char *omci = NULL;
    int i = 0;
    int len = 0;
    int nRow = 0;
    //DEBUG_COLOR(LIGHT_CYAN, "Infile : %s", Infile);

    printf("\n\n\n\n\nWelcome OMCI Parser\n\n");
    dbg_omci_me_modules_list();

    infp = fopen(Infile,"r");
    if(infp == NULL){
        ERROR("fopen %s failed",Infile);
        return INTERNAL_ERROR;
    }

    while( fgets (buf, BUFLEN_256, infp)!=NULL ){
        //DEBUG("Omci Data[%d] = %s",i,buf);
        omci = strtrimc(buf);
        buf[80]='\0';

        len = strlen(omci);
        //DEBUG("Trim blank => Omci Data[%03d] = %s",i,omci);
        parser_omci_msg(omci,i);
        i++;
        //reset buf
        memset(buf,0,sizeof(buf));
    }

    printf("\n");
    NOTICE("Total OMCI number : %d",i);

    fclose(infp);
    return CMSRET_SUCCESS;
}

int main(int argc,char **argv)
{
    char *Infile = NULL;
    char *Outfile = NULL;
    Ret ret = CMSRET_SUCCESS;
        
    analyze_parameter(argc,argv);
    Infile = argv[1];

    ret = omciparser(Infile);
    if(ret != CMSRET_SUCCESS){
        ERROR("omciparser failed , ret = %d",ret);
    }
    return 0;
}
