/*
    util - helper functions
    Copyright (C) 2006-2009  Jean Delvare <jdelvare@suse.de>

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

#ifndef _UTIL_H
#define _UTIL_H

extern int user_ack(int def);

//#ifdef ODM_GEMTEK

#define E2PROM_PARM_LEN  32
#define RET_FAILED       (-1)
#define RET_SUCCESS      1
#define HEX_MODE         0
#define CHARACTER_MODE   1

typedef struct {
    char parm[32];
    unsigned char initialAddr;
    int length;
    int printMode; // 0 is hex , 1 is character
    void (*set)(char *,char *,int *);
} E2PROM_DATA_MAP;

extern E2PROM_DATA_MAP nokia_ri[];
extern int find_nokia_RI_entry(char *parm);
extern void print_all_support_parm(void);
//#endif //ODM_GEMTEK

#endif /* _UTIL_H */
