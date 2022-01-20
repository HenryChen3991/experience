#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "omci.h"

static void analyze_parameter(int argc, char *argv[],int *mode,int *filter_me);

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

static void analyze_parameter(int argc, char *argv[],int *mode,int *filter_me)
{
    int i = 0;
    if(argc < 2 || access(argv[1],R_OK) != CMSRET_SUCCESS){
        usage();
        exit(0);
    }

    if(!cmsUtl_strcmp(argv[2], "-f")){
        *mode = FILTER;
        *filter_me = atoi(argv[3]);
        DEBUG("mode = %s , filter_me = %d",omciParser_mode_name[*mode],*filter_me);
        i+=2;
    }
    if(!cmsUtl_strcmp(argv[2], "-MEReplay")){
        *mode = MEREPLAY;
        DEBUG("mode = %s",omciParser_mode_name[*mode]);
        i+=1;
    }
}

Ret omciparser(char *Infile,int mode,int filter_me){
    FILE *infp = NULL;
    Ret ret = CMSRET_SUCCESS;
    char buf[BUFLEN_256] = {0};
    char *omci = NULL;
    int i = 0;
    int len = 0;
    int nRow = 0;
    //DEBUG_COLOR(LIGHT_CYAN, "Infile : %s", Infile);

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
        parser_omci_msg(omci,i,mode,filter_me);
        i++;
        //reset buf
        memset(buf,0,sizeof(buf));
    }

    printf("\n");
    NOTICE("Total OMCI number : %d",i);

    fclose(infp);
    return CMSRET_SUCCESS;
}

Ret omcireplay(char *Infile)
{
    FILE *infp = NULL;
    FILE *outfp = NULL;
    Ret ret = CMSRET_SUCCESS;
    char buf[BUFLEN_256] = {0};
    char outFile[BUFLEN_128] = {0};
    char *omci = NULL;
    int i = 0;
    int match_counter = 0;

    snprintf(outFile,sizeof(outFile),"%s-replay",Infile);
    infp = fopen(Infile,"r");
    if(infp == NULL){
        ERROR("fopen %s failed",Infile);
        return INTERNAL_ERROR;
    }
    
    outfp = fopen(outFile,"w");
    if(outfp == NULL){
        ERROR("fopen %s failed",outFile);
        fclose(infp);
        return INTERNAL_ERROR;
    }

    while( fgets (buf, BUFLEN_256, infp)!=NULL ){
        //DEBUG("Omci Data[%d] = %s",i,buf);
        omci = strtrimc(buf);
        buf[80]='\0';

        //DEBUG("Trim blank => Omci Data[%03d] = %s",i,omci);
        ret = replay_omci_msg(omci,i++,outfp);
        if(ret == MATCH_MEREPLAY_RULES)
            match_counter++;
        //reset buf
        memset(buf,0,sizeof(buf));
    }
    NOTICE("Total OMCI match number : %d",match_counter);

    fclose(infp);
    fclose(outfp);
    return CMSRET_SUCCESS;
}

int main(int argc,char **argv)
{
    char *Infile = NULL;
    int mode = NORMANL;
    int filter_me = 0;
    Ret ret = CMSRET_SUCCESS;
    printf("\n\n\n\n\nWelcome OMCI Parser\n\n");
    analyze_parameter(argc,argv,&mode,&filter_me);
    Infile = argv[1];

    switch (mode)
    {
        case NORMANL:
            ret = omciparser(Infile,mode,0);
            if(ret != CMSRET_SUCCESS){
                ERROR("omciparser %s failed , ret = %d",omciParser_mode_name[mode],ret);
            }
            break;
        case FILTER:
            ret = omciparser(Infile,mode,filter_me);
            if(ret != CMSRET_SUCCESS){
                ERROR("omciparser %s failed , ret = %d",omciParser_mode_name[mode],ret);
            }
            break;
        case MEREPLAY:
            ret = omcireplay(Infile);
            if(ret != CMSRET_SUCCESS){
                ERROR("omciparser %s failed , ret = %d",omciParser_mode_name[mode],ret);
            }
            break;
        default:
            ERROR("Unknown omciparser mode");
            break;
    }
    
    return 0;
}
