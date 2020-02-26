# include "bd1.h"
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
	char personalization[] = "bd1 test";
	unsigned char data_rec[2*LEN],data_sent[LEN],buf[10000];
	bd1_context ctx;
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

	if ((ret = bd1_init(&ctx,grp_id)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	if ((ret = bd1_gen_point(&ctx,&olen,data_sent,LEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	sendData(0,0,0,data_sent,LEN,UART1);
	receiveData(data_rec,(N-1)*LEN);
	turnGPIO(PORTD,15,ON);

	memcpy(data_rec+LEN,data_rec,LEN);
	if ((ret = bd1_gen_value(&ctx,data_rec,2*LEN,&olen,data_sent,LEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	if ((ret = bd1_compute_key(&ctx,data_rec,LEN,2,&olen,data_sent,LEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}
	
	turnGPIO(PORTD,12,OFF);
	turnGPIO(PORTD,15,OFF);
	
	entropy_free(&entropy);
	bd1_free(&ctx);

	return 0;
}
