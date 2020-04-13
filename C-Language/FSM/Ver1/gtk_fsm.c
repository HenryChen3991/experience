#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gtk_fsm.h"


//SwdlTable size
int q_max_num = 0;

typedef enum
{
    IDLE,
    START_SW_DOWNLOAD,
    END_SW_DOWNLOAD,
    ACTIVATE_IMAGE,
    COMMIT_IMAGE
} swdl_state_e;

static char *swdl_state_name[] ={
    "IDLE",
    "START_SW_DOWNLOAD",
    "END_SW_DOWNLOAD",
    "ACTIVATE_IMAGE",
    "COMMIT_IMAGE"
};
static int swdl_state_name_size = sizeof(swdl_state_name)/sizeof(char *);

typedef enum
{
    AA,
    BB,
    CC,
    DD,
    EE,
    FF
} swdl_event_e;

static char *swdl_event_name[] ={
    "AA",
    "BB",
    "CC",
    "DD",
    "EE",
    "FF"
};

static int swdl_event_name_size = sizeof(swdl_event_name)/sizeof(char *);

typedef struct
{
    swdl_state_e currentState;
    swdl_event_e event;
    swdl_state_e nextState;
    void (*handler)(struct eocStack *stack);
} swdl_table_t;


typedef struct stateMachine
{
    swdl_state_e state;
    swdl_table_t *swdlTable;
    struct eocStack *stack;
} gtk_fsm_t;

void FSM_init(gtk_fsm_t *pfsm, swdl_table_t *swdlTable,swdl_state_e state,struct eocStack *stack);
void FSM_StateTransfer(gtk_fsm_t *pfsm,swdl_state_e state);
int  FSM_EventHandler(gtk_fsm_t *pfsm,swdl_event_e event);
void FSM_ParseRegisterEventNum();

//Testing
void do_b(struct eocStack *stack);
void do_c(struct eocStack *stack);

//Optmize
struct table_index
{
    swdl_state_e state;
    int start;
    int end;
};

struct table_index register_event_num[] =
{
    {IDLE,0,0},
    {START_SW_DOWNLOAD,0,0},
    {END_SW_DOWNLOAD,0,0},
    {ACTIVATE_IMAGE,0,0},
    {COMMIT_IMAGE,0,0}
};


/*

Ex:
                                    event  'AA'
   +------------------------------------------------------------------------------+
   |                               handler 'NULL'                                 |
   v                                                                              |
+----+      event  'AA'      +-----------------+       event  'BB'         +---------------+     event  'CC'      +--------------+     event  'DD'       +------------+
|IDLE| --------------------> |START_SW_DOWNLOAD| ------------------------> |END_SW_DOWNLOAD| -------------------> |ACTIVATE_IMAGE| --------------------> |COMMIT_IMAGE|
+----+    handler 'NULL'     +-----------------+      handler 'do_b'       +---------------+    handler 'do_c'    +--------------+    handler 'NULL'     +------------+
   ^                                                                                                                                                            |
   |                                                                    event  'EE'                                                                             |
   +------------------------------------------------------------------------------------------------------------------------------------------------------------+
                                                                       handler 'NULL'
*/
swdl_table_t SwdlTable[] = 
{
//  currentState            ,event          ,nextState                ,handler

//IDLE
    {IDLE                   ,AA             ,START_SW_DOWNLOAD        ,NULL},

//START_SW_DOWNLOAD
    {START_SW_DOWNLOAD      ,BB             ,END_SW_DOWNLOAD          ,do_b},

//END_SW_DOWNLOAD
    {END_SW_DOWNLOAD        ,CC             ,ACTIVATE_IMAGE           ,do_c},
    {END_SW_DOWNLOAD        ,FF             ,IDLE                     ,NULL},

//ACTIVATE_IMAGE
    {ACTIVATE_IMAGE         ,DD             ,COMMIT_IMAGE             ,NULL},

//COMMIT_IMAGE
    {COMMIT_IMAGE           ,EE             ,IDLE                     ,NULL}
};

void FSM_ParseRegisterEventNum()
{
    int i = 0;
    int start = 0;
    int end = 0;
    int num[swdl_state_name_size];

    for( i = 0 ; i < swdl_state_name_size ; i++)
        num[i] = 0;

    for ( i = 0 ; i < q_max_num ; i++){
        num[SwdlTable[i].currentState]++;
    }

#if 0
    for(i=0;i<swdl_state_name_size;i++)
        DEBUG("num[%d] = %d",i,num[i]);
#endif //0

    for( i = 0 ; i < swdl_state_name_size ; i++){
        if(num[i]==0){
            register_event_num[i].start = -1;
            register_event_num[i].end = -1;
        }
        else{
            end = start + num[i]-1;
            register_event_num[i].start = start;
            register_event_num[i].end = end;
            start += num[i];
        }
    }

#if 0
    for( i = 0 ; i < sizeof(register_event_num)/sizeof(struct table_index) ; i++)
    {
        DEBUG("%25s , start index = %3d , end index = %3d",swdl_state_name[i],register_event_num[i].start,register_event_num[i].end);
    }
#endif //0
}

void FSM_init(gtk_fsm_t *pfsm, swdl_table_t *swdlTable,swdl_state_e state,struct eocStack *stack)
{
    pfsm->swdlTable = swdlTable;
    pfsm->state = state;
    pfsm->stack = stack;
    q_max_num = sizeof(SwdlTable)/sizeof(swdl_table_t);

    DEBUG("Enter");

    FSM_ParseRegisterEventNum();
}

void FSM_StateTransfer(gtk_fsm_t *pfsm,swdl_state_e state)
{
    DEBUG("NextState is %s",swdl_state_name[state]);
    pfsm->state = state;
}

int FSM_EventHandler(gtk_fsm_t *pfsm,swdl_event_e event)
{
    swdl_table_t *swdlTable = pfsm->swdlTable;
    void (*handler)(struct eocStack *stack) = NULL;
    struct eocStack *stack = pfsm->stack;
    swdl_state_e nextState;
    swdl_state_e currentState = pfsm->state;
    int rc = FAILED;
    int i = 0;
    int start = register_event_num[currentState].start;
    int end = register_event_num[currentState].end;

    if( event < 0 || event >= swdl_event_name_size ||
        pfsm->state < 0 || pfsm->state >= swdl_state_name_size){
        ERROR("Having some error state or event coming");
        return rc;
    }
    else{
        DEBUG("current state %s and event %s is coming",swdl_state_name[pfsm->state],swdl_event_name[event]);
    }

    //find specificed event and state handler
    for(i = start && start != -1 ; i <= end ; i++){
        if( event == swdlTable[i].event && currentState == swdlTable[i].currentState){
            handler = swdlTable[i].handler;
            nextState = swdlTable[i].nextState;
            rc = SUCCESS;
//            DEBUG("found specificed event and handler");
        }
    }

    //success
    if(rc == SUCCESS){
        //execute handler
        if(handler != NULL){
            handler(stack);
        }
        //transfer next state
        FSM_StateTransfer(pfsm,nextState);
    }
    else {
        DEBUG("No specificed machine state and event!!!, keeping originial state %s",swdl_state_name[pfsm->state]);
    }

    return rc;
}

//测试用的
void do_b(struct eocStack *stack){
    DEBUG("doing b..., seq = %d",stack->seq);
    stack->seq++;
}

void do_c(struct eocStack *stack){
    DEBUG("doing c..., seq = %d",stack->seq);
    stack->seq++;
}

int flag = 1;
void test(int *event)
{
    if (*event == FF)
    {
        *event = AA;
    }
    else if (*event == BB && flag ==1 ){
        *event = FF;
        flag = 0;
    }
    else
    {
        (*event)++;
    }

}

void main(void)
{
    struct eocStack stack;
    gtk_fsm_t sdwl_fsm;
    FSM_init(&sdwl_fsm,SwdlTable,IDLE,&stack);
    
    int state = IDLE;
    int event = AA;
    stack.seq=0;

#if 0
    DEBUG("swdl_state_name_size = %d",swdl_state_name_size);
    DEBUG("swdl_event_name_size = %d",swdl_event_name_size);
#endif //0

    while(1){
        FSM_EventHandler(&sdwl_fsm,event);
        test(&event);
        sleep(2); //2s
    }
}
