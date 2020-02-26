//extern static int OLT_TYPE;


typedef struct
{
        char olt_mac[3];
} oltPrototypeMac_t;


typedef struct
{
	int num;
	oltPrototypeMac_t *olt_prototype_ptr;
} olt_prototype_t;

typedef struct
{
	oltPrototypeMac_t olt_prototype[2];
} rOltPrototype_t;

typedef struct
{
	oltPrototypeMac_t olt_prototype[3];
} sumitomoOltPrototype_t;

typedef struct
{
	oltPrototypeMac_t olt_prototype[1];
} nokia7360OltPrototype_t;

/*
typedef struct
{
	rOltPrototype_t	        *nokia_r;
	sumitomoOltPrototype_t  *sumitomo;
	nokia7360OltPrototype_t *nokia_7360;
} olt_prototype_t;
*/

typedef enum
{
    Nokia_R_OLT         = 0,
    Sumitomo_OLT        = 1,
    Nokia_7360_OLT      = 2,
    Unknown_OLT         = 3,
    No_Connect_OLT      = 4, //initial status
} olt_type_e;
#define oltPrototypeNum 3

int check_olt_prototype(char *str);
void print_current_olt_prototype(void);
int getOltType(void);
int setOltType(olt_type_e type);
extern rOltPrototype_t R_OltPrototype;
void print_mac(char *str);
void new_check(char *str);
