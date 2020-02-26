#define E2PROM_PARM_LEN 32
#define RET_FAILED  (-1)
#define RET_SUCCESS 1
typedef struct {
    char parm[E2PROM_PARM_LEN];
    unsigned char initialAddr;
    int length;
    int (*set)(int , char *,char *, int *);
} E2PROM_DATA_MAP;

extern E2PROM_DATA_MAP nokia_ri[];
int find_nokia_RI_entry(char *parm);
void print_all_support_parm(void);
