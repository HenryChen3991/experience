
#include<stdio.h>
#include<stdlib.h>


//#pragma pack(push)  /* push current alignment to stack */
//#pragma pack(1)     /* set alignment to 1 16 byte boundary */


#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6

#define NVRAM_GPON_SERIAL_NUMBER_LEN    13
#define NVRAM_GPON_PASSWORD_LEN         11


#define  WLAN_FEATURE_DHD_NIC_ENABLE     0x01
#define  WLAN_FEATURE_DHD_MFG_ENABLE     0x02
#define  WLAN_FEATURE_LAST_FEATURE       0x80

#define  WLAN_MFG_PARTITION_ISNAND       0x01
#define  WLAN_MFG_PARTITION_MFGSET       0x02
#define  WLAN_MFG_PARTITION_HASSIZE      0x04
#define  WLAN_MFG_PARTITION_NAME         "misc3"
#define  WLAN_MFG_PARTITION_INDEX         2

#define NVRAM_WLAN_PARAMS_LEN      256
#define NVRAM_WPS_DEVICE_PIN_LEN   8

#define NVRAM_BOOTLDR_SIG_LEN           256
#define NVRAM_BOOTLDR_SIG_OFFSET        1024

#define NVRAM_CV_KEY_LEN                514
#define NVRAM_MFG_CV_KEY_OFFSET         1280
#define NVRAM_OP_CV_KEY_OFFSET          1794

#define NVRAM_ENC_KEY_LEN               32

#define NVRAM_BOOTLDR_ENC_KEY_OFFSET    2308
#define NVRAM_IMAGE_ENC_KEY_OFFSET      2340

#define NVRAM_ENC_IV_LEN                32

#define NVRAM_BOOTLDR_ENC_IV_OFFSET #define NP_BOOT             0   /*cferom*/
#define NP_NVRAM            1   /*NVRAM*/
#define NP_DATA             2   /*data*/
#define NP_JFFS2_1          3   /*cferam0+kernel0*/
#define NP_JFFS2_2          4   /*cferam1+kernel1*/
#define NP_ROOTFS_1         5   /*rootfs0*/
#define NP_ROOTFS_2         6   /*rootfs1*/
#define NP_LOGFS            7   /*logfs*/
#define NP_DATAFS           8   /*datafs*/
#define NP_BBT              8   /*not really assign space in flash*/
#define NP_TOTAL            9   /*exclude BBT*/

#define NAND_DATA_SIZE_KB       4096
#define NAND_BBT_THRESHOLD_KB   (512 * 1024)
#define NAND_BBT_SMALL_SIZE_KB  1024
#define NAND_BBT_BIG_SIZE_KB    4096

#define NAND_CFE_RAM_NAME           "cferam.000"
#define NAND_CFE_RAM_SECBT_NAME     "secram.000"
#define NAND_RFS_OFS_NAME           "NAND_RFS_OFS"
#define NAND_COMMAND_NAME           "NANDCMD"
#define NAND_BOOT_STATE_FILE_NAME   "boot_state_x"
#define NAND_SEQ_MAGIC              0x53510000


#define BCM_MAX_EXTRA_PARTITIONS 4



struct allocs_rdp {
    unsigned char tmsize;
    unsigned char mcsize;
    unsigned char reserved[2];
};

struct allocs_dhd {
    unsigned char dhd_size[3];
    unsigned char reserved;
};



typedef struct
{
    unsigned int ulVersion;                                                                     //4
    char szBootline[NVRAM_BOOTLINE_LEN];                                                        //256
    char szBoardId[NVRAM_BOARD_ID_STRING_LEN];                                                  //16
    unsigned int ulMainTpNum;                                                                   //4
    unsigned int ulPsiSize;                                                                     //4
    unsigned int ulNumMacAddrs;                                                                 //4
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];                                        //6
    char pad;                                                                                   //1
    char backupPsi;  /**< if 0x01, allocate space for a backup PSI */                           //1
    unsigned int ulCheckSumV4;                                                                  //4
    char gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN];                                        //13
    char gponPassword[NVRAM_GPON_PASSWORD_LEN];                                                 //11
    char wpsDevicePin[NVRAM_WPS_DEVICE_PIN_LEN];                                                //8
    char wlanParams[NVRAM_WLAN_PARAMS_LEN];                                                     //256
    unsigned int ulSyslogSize; /**< number of KB to allocate for persistent syslog */           //4
    unsigned int ulNandPartOfsKb[NP_TOTAL];                                                     //36
    unsigned int ulNandPartSizeKb[NP_TOTAL];                                                    //36
    char szVoiceBoardId[NVRAM_BOARD_ID_STRING_LEN];                                             //16
    unsigned int afeId[2];                                                                      //8
    unsigned short opticRxPwrReading;   // optical initial rx power reading                     //2
    unsigned short opticRxPwrOffset;    // optical rx power offset                              //2
    unsigned short opticTxPwrReading;   // optical initial tx power reading                     //2
    unsigned char ucUnused2[58];                                                                //58
    unsigned char ucFlashBlkSize;                                                               //1
    unsigned char ucAuxFSPercent;                                                               //1

    unsigned int ulBoardStuffOption;   // board options. bit0-3 is for DECT                     //4    
    union {
        unsigned int reserved;
        struct allocs_rdp alloc_rdp;
    } allocs;                                                                                   //4
    unsigned int ulMemoryConfig;                                                                //4
    struct allocs_dhd alloc_dhd;                                                                //4
    /* Add any new non-secure related elements here */
    char chReserved[177];                                                                       //175   177

    struct partition_info {
        /*
                2MSB represent the 
                        00 = MB 
                        01 = GB
                        10 = reserved
                        11 = reserved
                14LSB represent multiple of 2MSB
        */

        unsigned short size;
    } part_info[BCM_MAX_EXTRA_PARTITIONS];                                                      //4*2 = 8

    char chBootFlag; //decide which system will be launched , 0 as Factory img, 1 as Custom img. 0 is default and would boot from tftp.         //1
    char chUnused[64]; /* Adjust chUnused such that everything above + chUnused[] + ulCheckSum = 1k */                                          //64

    unsigned int ulCheckSum;                                                                    //4
}  __attribute__ ((packed)) NVRAM_DATA, *PNVRAM_DATA;

#define X(name,parameter) sizeof(name->parameter)

void dumpNvramMemberSize(PNVRAM_DATA nvram)
{

        char *parameter[34] = { "ulVersion","szBootline","szBoardId",
                                "ulMainTpNum", "ulPsiSize", "ulNumMacAddrs","ucaBaseMacAddr","pad",
                                "backupPsi","ulCheckSumV4","gponSerialNumber","gponPassword",
                                "wpsDevicePin","wlanParams","ulSyslogSize","ulNandPartOfsKb",
                                "ulNandPartSizeKb","szVoiceBoardId","afeId","opticRxPwrReading",
                                "opticRxPwrOffset","opticTxPwrReading","ucUnused2","ucFlashBlkSize","ucAuxFSPercent",
                                "ulBoardStuffOption","allocs","ulMemoryConfig","alloc_dhd","chReserved","part_info",
                                "chBootFlag","chUnused","ulCheckSum" };
  
       int i=0; 
       int total_size=0;
       int size[34];
       size[0] = X(nvram,ulVersion);
       size[1] = X(nvram,szBootline);
       size[2] = X(nvram,szBoardId);
       size[3] = X(nvram,ulMainTpNum);
       size[4] = X(nvram,ulPsiSize);
       size[5] = X(nvram,ulNumMacAddrs);
       size[6] = X(nvram,ucaBaseMacAddr);
       size[7] = X(nvram,pad);
       size[8] = X(nvram,backupPsi);
       size[9] = X(nvram,ulCheckSumV4);
       size[10] = X(nvram,gponSerialNumber);
       size[11] = X(nvram,gponPassword);
       size[12] = X(nvram,wpsDevicePin);
       size[13] = X(nvram,wlanParams);
       size[14] = X(nvram,ulSyslogSize);
       size[15] = X(nvram,ulNandPartOfsKb);
       size[16] = X(nvram,ulNandPartSizeKb);
       size[17] = X(nvram,szVoiceBoardId);
       size[18] = X(nvram,afeId);
       size[19] = X(nvram,opticRxPwrReading);
       size[20] = X(nvram,opticRxPwrOffset);
       size[21] = X(nvram,opticTxPwrReading);
       size[22] = X(nvram,ucUnused2);
       size[23] = X(nvram,ucFlashBlkSize);
       size[24] = X(nvram,ucAuxFSPercent);
       size[25] = X(nvram,ulBoardStuffOption);
       size[26] = X(nvram,allocs);
       size[27] = X(nvram,ulMemoryConfig);
       size[28] = X(nvram,alloc_dhd);
       size[29] = X(nvram,chReserved);
       size[30] = X(nvram,part_info);
       size[31] = X(nvram,chBootFlag);
       size[32] = X(nvram,chUnused);
       size[33] = X(nvram,ulCheckSum);
       for(i=0;i<34;i++){
           printf("parameter %20s size is %3d\n",(char *)parameter[i],size[i]);
           total_size += size[i];
       }

       printf("total_size = %d\n",total_size);
}




int main(void)
{
	
	printf("%d\n",sizeof(NVRAM_DATA));
	PNVRAM_DATA nvram;
	
	dumpNvramMemberSize(nvram);
	return 0;
}


/*
#if (INC_BTRM_BOOT==1) && (defined(_BCM963268_) || defined(CONFIG_BCM963268))
    unsigned int ulCheckSum_1k;
    char signature[NVRAM_BOOTLDR_SIG_LEN];
    char mfgCvKey[NVRAM_CV_KEY_LEN];
    char opCvKey[NVRAM_CV_KEY_LEN];
    char bek[NVRAM_ENC_KEY_LEN];
    char iek[NVRAM_ENC_KEY_LEN];
    char biv[NVRAM_ENC_IV_LEN];
    char iiv[NVRAM_ENC_IV_LEN];
    // Add any secure boot related elements here 
    char chSecUnused[632];
#endif
*/
