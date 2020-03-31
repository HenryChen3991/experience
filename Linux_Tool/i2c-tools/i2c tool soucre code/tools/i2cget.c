/*
    i2cget.c - A user-space program to read an I2C register.
    Copyright (C) 2005-2012  Jean Delvare <jdelvare@suse.de>

    Based on i2cset.c:
    Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
                             Mark D. Studebaker <mdsxyz123@yahoo.com>
    Copyright (C) 2004-2005  Jean Delvare

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/

#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "i2cbusses.h"
#include "util.h"
#include "../version.h"

static void help(void) __attribute__ ((noreturn));

static void help(void)
{
    fprintf(stderr,
//ifdef ODM_GEMTEK
            "Usage:\n"
            "  1. i2cget [-f] [-y] [-t delay(ms)] I2CBUS CHIP-ADDRESS [ DATA-ADDRESS [ MODE [LENGTH] ] ]\n"
            "  2. i2cget [-G parm]\n"
//#else
//            "Usage: i2cget [-f] [-y] I2CBUS CHIP-ADDRESS [DATA-ADDRESS [MODE]]\n"
//#endif //ODM_GEMTEK
            "  I2CBUS is an integer or an I2C bus name\n"
            "  ADDRESS is an integer (0x03 - 0xff)\n"
            "  MODE is one of:\n"
            "    b (read byte data, default)\n"
            "    w (read word data)\n"
            "    c (write byte/read byte)\n"
//#ifdef ODM_GEMTEK
            "    g [LENGTH] (GEMTEK mode,length default is 1)\n"
//#endif //ODM_GEMTEK
            "    Append p for SMBus PEC\n");
//#ifdef ODM_GEMTEK
    print_all_support_parm();
//#endif //ODM_GEMTEK
    exit(1);
}

static int check_funcs(int file, int size, int daddress, int pec)
{
    unsigned long funcs;

    /* check adapter functionality */
    if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
        fprintf(stderr, "Error: Could not get the adapter "
                "functionality matrix: %s\n", strerror(errno));
        return -1;
    }

    switch (size) {
        case I2C_SMBUS_BYTE:
            if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus receive byte");
                return -1;
            }
            if (daddress >= 0
                    && !(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
                return -1;
            }
            break;

        case I2C_SMBUS_BYTE_DATA:
            if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus read byte");
                return -1;
            }
            break;

        case I2C_SMBUS_WORD_DATA:
            if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus read word");
                return -1;
            }
            break;
    }

    if (pec
            && !(funcs & (I2C_FUNC_SMBUS_PEC | I2C_FUNC_I2C))) {
        fprintf(stderr, "Warning: Adapter does "
                "not seem to support PEC\n");
    }

    return 0;
}

static int confirm(const char *filename, int address, int size, int daddress,
                   int pec)
{
    int dont = 0;

    fprintf(stderr, "WARNING! This program can confuse your I2C "
            "bus, cause data loss and worse!\n");

    /* Don't let the user break his/her EEPROMs */
    if (address >= 0x50 && address <= 0x57 && pec) {
        fprintf(stderr, "STOP! EEPROMs are I2C devices, not "
                "SMBus devices. Using PEC\non I2C devices may "
                "result in unexpected results, such as\n"
                "trashing the contents of EEPROMs. We can't "
                "let you do that, sorry.\n");
        return 0;
    }

    if (size == I2C_SMBUS_BYTE && daddress >= 0 && pec) {
        fprintf(stderr, "WARNING! All I2C chips and some SMBus chips "
                "will interpret a write\nbyte command with PEC as a"
                "write byte data command, effectively writing a\n"
                "value into a register!\n");
        dont++;
    }

    fprintf(stderr, "I will read from device file %s, chip "
            "address 0x%02x, ", filename, address);
    if (daddress < 0)
        fprintf(stderr, "current data\naddress");
    else
        fprintf(stderr, "data address\n0x%02x", daddress);
    fprintf(stderr, ", using %s.\n",
            size == I2C_SMBUS_BYTE ? (daddress < 0 ?
                                      "read byte" : "write byte/read byte") :
            size == I2C_SMBUS_BYTE_DATA ? "read byte data" :
            "read word data");
    if (pec)
        fprintf(stderr, "PEC checking enabled.\n");

//#ifdef ODM_GEMTEK
//    fprintf(stderr, "Continue? [%s] ", dont ? "y/N" : "Y/n");
//#endif //ODM_GEMTEK
    fflush(stderr);
//#ifdef ODM_GEMTEK
//#else
//        if (!user_ack(1)) {
//            fprintf(stderr, "Aborting on user request.\n");
//            exit(0);
//        }
//#endif //ODM_GEMTEK

    return 1;
}

int main(int argc, char *argv[])
{
    char *end;
    int res, i2cbus, address, size, file;
    int daddress;
    char filename[20];
    int pec = 0;
    int flags = 0;
    int force = 0, yes = 0, version = 0;
//#ifdef ODM_GEMTEK
    int gtk_mode = 0;
    int delay = 10000; //10ms
    char parm[E2PROM_PARM_LEN] = {0};
    int e2prom_parm_index = -1;
//#endif //ODM_GEMTEK

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
//#ifdef ODM_GEMTEK
            case 't':
                if ( 2+flags < argc )
                    delay = strtol(argv[2+flags],NULL,10)*1000;
                if (delay <= 0)
                    fprintf(stderr, "WARNING: delay <= 0 will casue reading failed problem, and suggest set 10 ms or use default value\n");
                flags++;
                break;
            case 'G':
                force = 1;
                yes = 1;
                gtk_mode = 2;
                if( argv[2+flags] == NULL ){
                    help();
                    exit(1);
                }
                strncpy(parm,argv[2+flags],sizeof(parm));
                flags++;
                break;
//#endif //ODM_GEMTEK
            default:
                fprintf(stderr, "Error: Unsupported option "
                        "\"%s\"!\n", argv[1+flags]);
                help();
                exit(1);
        }
        flags++;
    }

//#ifdef ODM_GEMTEK
    if(gtk_mode==2){
        if( (e2prom_parm_index = find_nokia_RI_entry(parm)) < 0){
            fprintf(stderr,"Not support parm : %s\n",parm);
            print_all_support_parm();
            exit(1);
        }
        i2cbus = lookup_i2c_bus("0");
        if(!strncmp(parm,"RollbackFlag",E2PROM_PARM_LEN))
            address = parse_i2c_address("0x57");
        else
            address = parse_i2c_address("0x56");
        size = I2C_SMBUS_BYTE_DATA;
        daddress = nokia_ri[e2prom_parm_index].initialAddr;
    }
    else{
//#endif //ODM_GEMTEK
        if (version) {
            fprintf(stderr, "i2cget version %s\n", VERSION);
            exit(0);
        }

        if (argc < flags + 3)
            help();

        i2cbus = lookup_i2c_bus(argv[flags+1]);
        if (i2cbus < 0)
            help();

        address = parse_i2c_address(argv[flags+2]);
        if (address < 0)
            help();

        if (argc > flags + 3) {
            size = I2C_SMBUS_BYTE_DATA;
            daddress = strtol(argv[flags+3], &end, 0);
            if (*end || daddress < 0 || daddress > 0xff) {
                fprintf(stderr, "Error: Data address invalid!\n");
                help();
            }
        }
        else {
            size = I2C_SMBUS_BYTE;
            daddress = -1;
        }

        if (argc > flags + 4) {
            switch (argv[flags+4][0]) {
                case 'b':
                    size = I2C_SMBUS_BYTE_DATA;
                    break;
                case 'w':
                    size = I2C_SMBUS_WORD_DATA;
                    break;
                case 'c':
                    size = I2C_SMBUS_BYTE;
                    break;
//#ifdef ODM_GEMTEK
                case 'g':
                    gtk_mode = 1;
                    break;
//#endif //ODM_GEMTEK
                default:
                    fprintf(stderr, "Error: Invalid mode!\n");
                    help();
            }
            pec = argv[flags+4][1] == 'p';
        }
//#ifdef ODM_GEMTEK
    }

    int gtk_len = 1;
    if(gtk_mode == 1) {
        if (argc > flags + 5) {
            gtk_len = strtol(argv[flags+5], &end, 0);
            if( *end || gtk_len <= 0 || gtk_len > 32 ) {
                fprintf(stderr,"Error: Length invalid! must be 1~32\n");
                help();
            }
        }
    }
    else if(gtk_mode ==2){
        gtk_len = nokia_ri[e2prom_parm_index].length;
    }

    unsigned char gtk_block[32]= {0};
    int stored_length = 0;
    file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
    do {
//#endif //ODM_GEMTEK
        if (file < 0
                || check_funcs(file, size, daddress, pec)
                || set_slave_addr(file, address, force))
            exit(1);

        if (!yes && !confirm(filename, address, size, daddress, pec))
            exit(0);

        if (pec && ioctl(file, I2C_PEC, 1) < 0) {
            fprintf(stderr, "Error: Could not set PEC: %s\n",
                    strerror(errno));
            close(file);
            exit(1);
        }

        switch (size) {
            case I2C_SMBUS_BYTE:
                if (daddress >= 0) {
                    res = i2c_smbus_write_byte(file, daddress);
                    if (res < 0)
                        fprintf(stderr, "Warning - write failed\n");
                }
                res = i2c_smbus_read_byte(file);
                break;
            case I2C_SMBUS_WORD_DATA:
                res = i2c_smbus_read_word_data(file, daddress);
                break;
            default: /* I2C_SMBUS_BYTE_DATA */
                res = i2c_smbus_read_byte_data(file, daddress);
        }
//#ifdef ODM_GEMTEK
//#else
//        close(file);
//#endif //ODM_GEMTEK

        if (res < 0) {
            fprintf(stderr, "Error: Read failed\n");
            close(file);
            exit(2);
        }

//#ifdef ODM_GMETEK
        gtk_block[stored_length] = res;
        stored_length += 1;
        gtk_len -= 1;
        daddress += 1;
        usleep(delay);
//#else
//    	printf("0x%0*x\n", size == I2C_SMBUS_WORD_DATA ? 4 : 2, res);
//#endif //ODM_GEMTEK

//#ifdef ODM_GEMTEK
    } while(gtk_len!=0);

    int i=0;
    if(nokia_ri[e2prom_parm_index].printMode == HEX_MODE){
        for(i=0; i<stored_length; i++)
            printf("0x%0*x ", size == I2C_SMBUS_WORD_DATA ? 4 : 2, gtk_block[i]);
    }
    else if(nokia_ri[e2prom_parm_index].printMode == CHARACTER_MODE){
        for(i=0; i<stored_length; i++){
            if( gtk_block[i] > 0x1F && gtk_block[i] < 0x7F )
                printf("%c",gtk_block[i]);
            else
                printf(" ");
        }
    }
    printf("\n");
    close(file);
//#endif //ODM_GEMTEK

    exit(0);
}
