# include "bd2.h"
# include "polarssl/entropy.h"
# include "polarssl/memory_buffer_alloc.h"
# include "stm32f4d_helper.h"
# include "KnxTpUart.h"
# include <string.h>

# define N      2
# define LEN    66
# define area   0
# define line   0
# define member 10

int notmain() {
	unsigned int olen;
	int ret;
	char personalization[] = "bd2 test";
	unsigned char data_sent[LEN],data_rec[LEN],buf[10000];
	bd2_context ctx;
	ctr_drbg_context ctr_drbg;
    entropy_context entropy;
	ecp_group_id grp_id = POLARSSL_ECP_DP_SECP256R1;
	f_source_ptr callback = &random;

	// set up dynamic memory management
	memory_buffer_alloc_init(buf,sizeof(buf));

	// initializing random number generator
	entropy_init(&entropy);
	entropy_add_source(&entropy,callback,NULL,4);
    if ((ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                               (const unsigned char *) personalization,
                               strlen(personalization))) != 0)
	{
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	inizializzaKnxTpUart(area,line,member);
	setListenToBroadcasts(true);

	turnGPIO(PORTD,12,ON);

	if ((ret = bd2_init(&ctx,grp_id)) != 0) {
		turnGPIO(PORTD,14,ON);
		entropy_free(&entropy);
		return (ret);
	}

	if ((ret = bd2_make_public(&ctx,&olen,data_sent,LEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
		entropy_free(&entropy);
		return (ret);
	}

	sendData(0,0,0,data_sent,LEN,UART1);
	receiveData(data_rec,LEN);
	turnGPIO(PORTD,15,ON);

	if ((ret = bd2_read_public(&ctx,data_rec,LEN)) != 0) {
		turnGPIO(PORTD,14,ON);
		entropy_free(&entropy);
		return (ret);
	}

	receiveData(data_rec,LEN);

	if ((ret = bd2_import_val(&ctx,data_sent,LEN)) != 0) {
		turnGPIO(PORTD,14,ON);
		entropy_free(&entropy);
		return (ret);
	}
	
	if ((ret = bd2_calc_key(&ctx,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
		entropy_free(&entropy);
		return (ret);
	}

	turnGPIO(PORTD,12,OFF);
	turnGPIO(PORTD,15,OFF);

	entropy_free(&entropy);
	bd2_free(&ctx);

	return 0;
}
