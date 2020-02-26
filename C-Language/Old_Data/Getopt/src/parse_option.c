#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static void help(void)
{
    fprintf(stderr,
            "Usage: i2cset [-f] [-y] [-m MASK] [-r] I2CBUS CHIP-ADDRESS DATA-ADDRESS [VALUE] ... [MODE]\n"
            "  I2CBUS is an integer or an I2C bus name\n"
            "  ADDRESS is an integer (0x03 - 0xff)\n"
            "  MODE is one of:\n"
            "    c (byte, no value)\n"
            "    b (byte data, default)\n"
            "    w (word data)\n"
            "    i (I2C block data)\n"
            "    s (SMBus block data)\n"
            "    Append p for SMBus PEC\n");
    exit(1);
}

void main(int argc, char *argv[])
{
	char *end;
    const char *maskp = NULL;
    int res, i2cbus, address, size, file;
    int value, daddress, vmask = 0;
    char filename[20];
    int pec = 0;
    int flags = 0;
    int force = 0, yes = 0, version = 0, readback = 0;
    int len;
    int gtk_flag = 0;
    unsigned char block[32]= {0};
    int i =0;
    int gMask = 0xF0;

    /* handle (optional) flags first */
    while (1+flags < argc && argv[1+flags][0] == '-') {
        switch (argv[1+flags][1]) {
            case 'V':
                version = 1;
                break;
            case 'f':
                force = 1;
                break;
            case 'y':
                yes = 1;
                break;
            case 'm':
                if (2+flags < argc)
                    maskp = argv[2+flags];
                flags++;
                break;
            case 'r':
                readback = 1;
                break;
	    case 'g':
		gtk_flag = 1;
		break;
            default:
                fprintf(stderr, "Error: Unsupported option "
                        "\"%s\"!\n", argv[1+flags]);
                help();
                exit(1);
        }

        flags++;
    }

/*
    if (argc < flags + 4)
        help();
*/

    printf("parse end\n");


/*
    if(gtk_flag == 1){
	system("./bin/parse_option -h");
    }
*/	
    daddress = strtol(argv[flags+3], &end, 0);
    if (*end || daddress < 0 || daddress > 0xff) {
        fprintf(stderr, "Error: Data address invalid!\n");
        help();
    }
    else{
	printf("daddress = 0x%x\n",daddress);
	printf("end = %s\n",end);
    }

    for (len = 0; len + flags + 5 < argc; len++) {
        value = strtol(argv[flags + len + 4], &end, 0);
//	printf("value = 0x%x\n",value);
        if (*end || value < 0) {
           fprintf(stderr, "Error: Data value invalid!\n");
           help();
        }
        if (value > 0xff) {
           fprintf(stderr, "Error: Data value out of range!\n");
           help();
        }
        block[len] = value;
    }
    printf("len = %d\n",len);
    value = -1;

    for(i=0 ; i<32 ; i++){
	printf("0x%2x ",block[i]);
	if(i==15||i==31)
		printf("\n");
    }

    printf("0x%2x\n",daddress & gMask);
    printf("0x%2x\n",daddress | 0x0F);
    printf("0x%2x - 0x%2x + 1 = %u\n",daddress | 0x0F,daddress,((daddress | 0x0F)-(daddress)+1));
    printf("0x%2x\n",daddress + ((daddress | 0x0F)-(daddress)+1) );

/*
    if( gtk_flag == 1 ){
        char cmd1[128] = {0};
        char cmd2[128] = {0};
        int gap = (daddress | 0x0F) - daddress + 1;
	printf("len = %d , gap = %d\n",len,gap);
        if( len > gap ){
            for(i=0;i<len;i++){
                if( i < gap ){
		   printf("block[%d] = %x\n",i,block[i]);
                   snprintf(cmd1+strlen(cmd1),sizeof(cmd1)-strlen(cmd1),"0x%x ",block[i]);
                }
                else{
                   snprintf(cmd2+strlen(cmd2),sizeof(cmd2)-strlen(cmd2),"0x%x ",block[i]);
                }
            }
            fprintf(stderr,"cmd1 = %s\n",cmd1);
            fprintf(stderr,"cmd2 = %s\n",cmd2);
            exit(1);
        }
    }
*/

    if( gtk_flag == 1){
        unsigned char gtk_block[16]={0};
        int gtk_addr = daddress;
	int total = len;
	printf("initial gtk_addr = 0x%x\n",gtk_addr);
#if 1
        while(total != 0){
	    int gtk_len = 0;
	    printf("gtk_addr = 0x%x to 0x%x\n",gtk_addr,(gtk_addr | 0x0F));
            int gap = (gtk_addr | 0x0F) - gtk_addr + 1;
	    if(total > gap){
            	gtk_len = gap;
            	printf("process %d arguments\n",gap);
            }
	    else{
		gtk_len = total;
                printf("process %d arguments\n",gtk_len);
            }
            int i=0;
	    memmove(gtk_block,block+(len-total),gtk_len);
            for(i=0;i<gtk_len;i++)
	         printf("0x%2x ",gtk_block[i]);
	    printf("\n");

            gtk_addr = gtk_addr + gap;
	    total -= gtk_len;
	}
#endif 
    }


    printf("0x%0*x\n", 3, block[1]);

    exit(1);

    printf("end of exit\n");
}
