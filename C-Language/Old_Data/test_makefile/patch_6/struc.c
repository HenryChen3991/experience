#include <stdio.h>
#include <stdlib.h>

#define TAG_LEN         256
#define TAG_VER_LEN     4
#define SIG_LEN         20
#define SIG_LEN_2       14   // Original second SIG = 20 is now devided into 14 for SIG_LEN_2 and 6 for CHIP_ID
#define CHIP_ID_LEN     6	
#define IMAGE_LEN       10
#define ADDRESS_LEN     12
#define FLAG_LEN        2
#define TOKEN_LEN       20
#define BOARD_ID_LEN    16
#define IMAGE_VER_LEN   32
#define RESERVED_LEN    (TAG_LEN - TAG_VER_LEN - SIG_LEN - SIG_LEN_2 - CHIP_ID_LEN - BOARD_ID_LEN - \
                        (5*IMAGE_LEN) - (4*ADDRESS_LEN) - (3*FLAG_LEN) - (2*TOKEN_LEN) - IMAGE_VER_LEN)
typedef struct _FILE_TAG
{
    char tagVersion[TAG_VER_LEN];       // tag version.  Will be 2 here.
    char signiture_1[SIG_LEN];          // text line for company info
    char signiture_2[SIG_LEN_2];        // additional info (can be version number)
    char chipId[CHIP_ID_LEN];           // chip id 
    char boardId[BOARD_ID_LEN];         // board id
    char bigEndian[FLAG_LEN];           // if = 1 - big, = 0 - little endia of the host
    char totalImageLen[IMAGE_LEN];      // the sum of all the following length
    char cfeAddress[ADDRESS_LEN];       // if non zero, cfe starting address
    char cfeLen[IMAGE_LEN];             // if non zero, cfe size in clear ASCII text.
    char rootfsAddress[ADDRESS_LEN];    // if non zero, filesystem starting address
    char rootfsLen[IMAGE_LEN];          // if non zero, filesystem size in clear ASCII text.
    union {
        char kernelAddress[ADDRESS_LEN];    // if non zero, kernel starting address
        char bootfsAddress[ADDRESS_LEN];    // if non zero, eMMC bootfs starting address
    };
    union {
        char kernelLen[IMAGE_LEN];          // if non zero, kernel size in clear ASCII text.
        char bootfsLen[IMAGE_LEN];          // if non zero, eMMC bootfs size in clear ASCII text.
    };
        char bootfsLen[IMAGE_LEN];          // if non zero, eMMC bootfs size in clear ASCII text.
    };
        char bootfsLen[IMAGE_LEN];          // if non zero, eMMC bootfs size in clear ASCII text.
    };
    char imageSequence[FLAG_LEN * 2];   // incrments everytime an image is flashed
    char imageVersion[IMAGE_VER_LEN];   // image version
    union {
        char dtbAddress[ADDRESS_LEN];       // if non zero, device tree blob starting address
        char mdataAddress[ADDRESS_LEN];     // if non zero, metadata blob starting address
    };
    union {
        char dtbLen[IMAGE_LEN];             // if non zero, device tree blob size in clear ASCII text.
        char mdataLen[IMAGE_LEN];           // if non zero, metadata blob size in clear ASCII text.
    };
    char reserved[RESERVED_LEN];        // reserved for later use
    char imageValidationToken[TOKEN_LEN];// image validation token - can be crc, md5, sha;  for
                                        // now will be 4 unsigned char crc
    char tagValidationToken[TOKEN_LEN]; // validation token for tag(from signiture_1 to end of // mageValidationToken)
} FILE_TAG, *PFILE_TAG;
byte ftoken[256];

void main(){
  PFILE_TAG tag = (PFILE_TAG)ftoken;
  strncpy(tag->boardId,"singtel",16-1);
  printf("size = %s\n",sizeof(ftoken));

}
