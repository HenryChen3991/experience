//
// GTKCRC32.C   Calculate CRC 32.
// (C) 2020/07/01 Copyright 2020 by Gemtek Technology Co., Ltd.
//

#include "GtkCrc32.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "checksum.h"

static void Usage(char * vPtrProgName);

static void Usage(char * vPtrProgName)
{
   fprintf(stderr, "usage: %s <filename>\n", vPtrProgName);
   fprintf(stderr, "\n");
}

int main(int argc, char * argv[])
{
    FILE * PtrFile = (NULL);
    uint32_t Crc32Val = 0xFFFFFFFF;
    int32_t Data;
    int32_t ErrorFlag = (EFAULT);
    char FilePath[(GTKCRC32_FILE_PATH_SIZE)] = "";

    if (argc != 2)
    {
        Usage(argv[0]);
        exit(ErrorFlag);
    }

    memset(FilePath, 0, sizeof(FilePath));
    strncpy(FilePath, argv[1], sizeof(FilePath) - 1);
    if (FilePath[0] == 0)
    {
        fprintf(stderr, "ERROR: file\n");
        exit(ErrorFlag);
    }
    PtrFile = fopen(FilePath, "rb");
    if (PtrFile == (NULL))
    {
        fprintf(stderr, "ERROR: fopen %s\n", FilePath);
        exit(ErrorFlag);
    }
    while ((Data = fgetc(PtrFile)) != EOF)
    {
        Crc32Val = update_crc_32(Crc32Val, (unsigned char)Data);
    }
    if (PtrFile != (NULL))
    {
        fclose(PtrFile);
        PtrFile = (NULL);
    }
    Crc32Val ^= 0xFFFFFFFF;

    printf("CRC32 %u %d 0x%X\n", Crc32Val, Crc32Val ,Crc32Val);

    return 0;
}

