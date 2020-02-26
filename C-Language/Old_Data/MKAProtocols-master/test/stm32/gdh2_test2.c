# include "gdh2.h"
# include "polarssl/entropy.h"
# include "polarssl/memory_buffer_alloc.h"
# include "stm32f4d_helper.h"
# include "KnxTpUart.h"
# include <string.h>

# define N      3
# define LEN    66
# define BUFLEN N*LEN

# define area1   0
# define line1   0
# define member1 1
# define area2   0
# define line2   0
# define member2 2
# define area3   0
# define line3   0
# define member3 3

int notmain() {
	unsigned int olen;
	int ret;
	char personalization[] = "gdh2 test";
	unsigned char data_sent[BUFLEN],data_rec[BUFLEN],buf[15000];;
	gdh2_context ctx;
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

	inizializzaKnxTpUart(area3,line3,member3);

	turnGPIO(PORTD,12,ON);
	
	gdh2_init(&ctx,grp_id,N,&ctr_drbg);

	receiveData(data_rec,BUFLEN);
	turnGPIO(PORTD,15,ON);

	if ((ret = gdh2_make_val(&ctx,3,data_rec,BUFLEN,&olen,data_sent,BUFLEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	sendData(0,0,0,data_sent,olen,UART1);

	if ((ret = gdh2_export_key(&ctx,&olen,data_sent,BUFLEN)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}	

	turnGPIO(PORTD,12,OFF);
	turnGPIO(PORTD,15,OFF);

	entropy_free(&entropy);
	gdh2_free(&ctx);
	
	return 0;
}
