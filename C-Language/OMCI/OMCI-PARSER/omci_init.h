#ifndef OMCI_INIT_H
#define OMCI_INIT_H

//OMCI_COMPLETE_MSG_LEN is 53
#define OMCI_COMPLETE_MSG_LEN (OMCI_HEADER_LEN+OMCI_TCID_LEN+OMCI_MSG_TYPE_LEN+OMCI_DEVICEID_LEN+OMCI_MSG_ID_LEN+OMCI_MSG_CONTENT_LEN+OMCI_TRAILER_LEN)
//OMCI_USEFUL_MSG_LEN is 40
#define OMCI_USEFUL_MSG_LEN (OMCI_TCID_LEN+OMCI_MSG_TYPE_LEN+OMCI_DEVICEID_LEN+OMCI_MSG_ID_LEN+OMCI_MSG_CONTENT_LEN)
#define OMCI_HEADER_LEN (5)
#define OMCI_TCID_LEN (2)
#define OMCI_MSG_TYPE_LEN (1)
#define OMCI_DEVICEID_LEN (1)
#define OMCI_MSG_ID_LEN (4)
#define OMCI_MSG_CONTENT_LEN (32)
#define OMCI_TRAILER_LEN (8)

typedef struct
{
    unsigned short tcid;
    unsigned char msgType;
    unsigned char deviceId;
    unsigned short meid;
    unsigned short instance;
    unsigned char msgContect[OMCI_MSG_CONTENT_LEN];
} omci_t;

/*
TYPE = 0x48
     = 0x001010000
b0 = 0
b1 = 0
b2 = 0
b3 = 1
b4 = 0
b5 = 0
b6 = 1
b7 = 0
*/
typedef union msg_type_detail
{
    unsigned char c;
    struct{
        unsigned int mt :5;
        unsigned int ak :1;
        unsigned int ar :1;
        unsigned int db :1;
    };
} msg_type_detail_t;

typedef struct omci_action
{
    unsigned int  ar;
    unsigned int  ak;
    unsigned int  mt;
    unsigned char name[BUFLEN_32];
} omci_action_t;

extern const omci_action_t omci_action_list[];
extern const int omci_action_list_size;

//#define DBG_OMCI_ME_MODULES_LIST

typedef struct omci_me_modules
{
    unsigned char meName[BUFLEN_128];
    unsigned short meId;
    void (*parser)(omci_t *omci,const unsigned char *meName);
} omci_me_modules_t;

extern const omci_me_modules_t omci_me_modules_list[];
extern const int omci_me_modules_list_size;

void dbg_omci_me_modules_list(void);
void dbg_module_info(omci_me_modules_t *module);

typedef union VLANTaggingOperationTableMapping
{
    unsigned int word[4];
    struct{
        unsigned int word1Padding :12;
        unsigned int filterOuterTpidde :3;
        unsigned int filterOuterVid :13;
        unsigned int filterOuterPrio :4;

        unsigned int filterEtherType :4;
        unsigned int word2Padding :8;
        unsigned int filterInnerTpidde :3;
        unsigned int filterInnerVid :13;
        unsigned int filterInnerPrio :4;

        unsigned int treatmentOuterTpidde :3;
        unsigned int treatmentOuterVid :13;
        unsigned int treatmentOuterPrio :4;
        unsigned int word3Padding :10;
        unsigned int treatmentTagsRemove :2;

        unsigned int treatmentInnerTpidde :3;
        unsigned int treatmentInnerVid :13;
        unsigned int treatmentInnerPrio :4;
        unsigned int word4Padding :12;
    };
} VLANTaggingOperationTableMapping_t;

/**
 * ME PARSER FUNCTION
 */
void me_9_12_2_parser(omci_t *omci,const unsigned char *meName);
void me_9_3_13_parser(omci_t *omci,const unsigned char *meName);
void me_65305_parser(omci_t *omci,const unsigned char *meName);


/**
 * ME NUMBER DEFINE
 */
#define ME_9_12_2 (131)
#define ME_9_3_13 (171)
#define ME_65305 (65305)

#define ATTR_MASK_LEN (16)
#define ATTR_MASK_SIZE_BYTE (2)

/**
 * OMCI ATTR HANDLER
 */
typedef struct omci_attr_handler
{
    unsigned int  num;
    unsigned char name[BUFLEN_128];
    unsigned int  size;
    void (*handler)(unsigned char *content,unsigned char *attr_name,unsigned int size,int index);
} omci_attr_handler_t;

#define OMCI_PACKET_MT_MAX (32)

#define OLT_SET "OLT > Set"
#define ONU_SET_REP "ONU < Set"
#define OLT_CREATE "OLT > Create"

typedef enum{
    NORMANL = 0,
    FILTER = 1,
    MEREPLAY = 2
} omciparser_mode_e;

extern const char *omciParser_mode_name[];

#endif //OMCI_INIT_H

