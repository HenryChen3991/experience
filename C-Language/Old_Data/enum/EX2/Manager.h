//extern static int OLT_TYPE;

typedef struct
{
        char olt_mac[3];
} oltPrototype_t;

typedef enum
{
    Nokia_7360_OLT      = 0,
    Sumitomo_OLT        = 1,
    Nokia_R_OLT         = 2,
    Unknown_OLT         = 3,
    No_Connect_OLT      = 4, //initial status
} olt_type_e;
#define oltPrototypeNum 3

int check_olt_prototype(char *str);
void print_current_olt_prototype(void);
int getOltType(void);
int setOltType(olt_type_e type);
