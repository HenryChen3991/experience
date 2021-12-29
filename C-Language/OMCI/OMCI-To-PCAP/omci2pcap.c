#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ckl_debug.h"
#include "gtk_string.h"

static void analyze_parameter(int argc, char *argv[]);
static void write_end_packets(FILE *fp);

#define APP "omci2pcap"
#define WIRESHARK_HEADER "00 90 d0 00 00 00 0a 76 ff 0c 8d 60 88 b5 "
#define OMCI_TRAILER "00 00 00 28 dc a2 62 5e"
#define ROW_FORMAT "00%d0 "

//temp record, two char convert one byte
/*
 char aaa[]="48";
 char bbb = '\0';
 bbb = (aaa[0]-'0')*16;
 bbb += (aaa[1]-'0');
 printf("bbb2 = %2X\n",bbb); //bbb2 = 48
*/



//Transcation ID 2 
//Messgae type 1
//Device ID 1
//Message ID 4
//Message content 32
#define OMCI_PACKET_LEN (40)

void usage(void){
    printf(LIGHT_RED"Invalid parpmeters\n\n"NONECOLOR);
    printf("Usage:\n\t \
        ./%s [input] [output]\nex:\n\t \
        ./%s omci.txt omci.pcap\n",APP,APP);
}
static void analyze_parameter(int argc, char *argv[])
{
    if(argc < 3 || access(argv[1],R_OK) != CMSRET_SUCCESS){
        usage();
        exit(0);
    }
}

Ret omci2pcap(char *Infile,char *Outfile){
    FILE *infp = NULL;
    FILE *outfp = NULL;
    char buf[BUFLEN_256] = {0};
    char *omci = NULL;
    int i = 0;
    int len = 0;
    int nRow = 0;
    DEBUG_COLOR(LIGHT_CYAN, "Infile : %s, Outfile : %s", Infile,Outfile);

    infp = fopen(Infile,"r");
    outfp = fopen(Outfile,"w+");
    if(infp == NULL){
        ERROR("fopen %s failed",Infile);
        return INTERNAL_ERROR;
    }

    if(outfp == NULL){
        ERROR("fopen %s failed",Outfile);
        fclose(infp);
        return INTERNAL_ERROR;
    }

    while( fgets (buf, BUFLEN_256, infp)!=NULL ){
        DEBUG("Omci Data[%d] = %s",i,buf);
        omci = strtrimc(buf);
        buf[80]='\0';

        len = strlen(omci);
        DEBUG("Trim blank => Omci Data[%d] = %s ; len = %d",i,omci, len);
        
        if( len == (OMCI_PACKET_LEN*2) ){
            while( len>0 ){
                fprintf(outfp,ROW_FORMAT,nRow);
                if(nRow == 0){
                    fputs(WIRESHARK_HEADER,outfp);
                    fprintf(outfp,"%c%c %c%c",omci[0],omci[1],omci[2],omci[3]);
                    omci += 4;
                    len -= 4;
                }
                else if(nRow == 3){
                    fprintf(outfp,"%c%c %c%c %c%c %c%c %c%c %c%c ",omci[0],omci[1],omci[2],omci[3]
                                                                 ,omci[4],omci[5],omci[6],omci[7]
                                                                 ,omci[8],omci[9],omci[10],omci[11]);
                    len -= 12;
                    omci += 12;

                    fputs(OMCI_TRAILER,outfp);
                }
                else{
                    fprintf(outfp,"%c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c"
                                                                           ,omci[0],omci[1],omci[2],omci[3]
                                                                           ,omci[4],omci[5],omci[6],omci[7]
                                                                           ,omci[8],omci[9],omci[10],omci[11]
                                                                           ,omci[12],omci[13],omci[14],omci[15]
                                                                           ,omci[16],omci[17],omci[18],omci[19]
                                                                           ,omci[20],omci[21],omci[22],omci[23]
                                                                           ,omci[24],omci[25],omci[26],omci[27]
                                                                           ,omci[28],omci[29],omci[30],omci[31]);
                    omci += 32;
                    len -= 32;
                }
                nRow++;
                fputs("\n",outfp);
            }
        }
        fputs("\n",outfp);
        i++;
        //reset buf and nRow
        memset(buf,0,sizeof(buf));
        nRow = 0;
    }
    NOTICE("Total OMCI number : %d",i);
    write_end_packets(outfp);

    fclose(infp);
    fclose(outfp);
    return CMSRET_SUCCESS;
}


//I don't know text2pcap what happens, need to write a fake packet. 
static void write_end_packets(FILE *fp)
{
    if( fp != NULL){
        fputs("0000 00 90 d0 00 00 00 0a 76 ff 0c 8d 60 88 b5 55 d8\n",fp);
        fputs("0010 48 0a 01 00 00 00 06 00 00 00 00 00 00 00 00 00\n",fp);
        fputs("0020 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00\n",fp);
        fputs("0030 00 00 00 00 00 00 00 00 00 28 dc a2 62 5e\n",fp);
    }
    else{
        ERROR("write_end_packets failed");
    }
}

int main(int argc,char **argv)
{
    char *Infile = NULL;
    char *Outfile = NULL;
    Ret ret = CMSRET_SUCCESS;
        
    analyze_parameter(argc,argv);
    Infile = argv[1];
    Outfile = argv[2];

    ret = omci2pcap(Infile,Outfile);
    if(ret != CMSRET_SUCCESS){
        ERROR("omci2pcap failed , ret = %d",ret);
    }
    return 0;
}
