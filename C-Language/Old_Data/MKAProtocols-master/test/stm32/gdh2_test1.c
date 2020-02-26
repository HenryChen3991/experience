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
	unsigned char data_sent1[BUFLEN],data_rec1[BUFLEN],data_sent2[BUFLEN],
                  data_rec2[BUFLEN],buf[15000];;
	gdh2_context ctx1,ctx2;
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

	inizializzaKnxTpUart(area1,line1,member1);

	turnGPIO(PORTD,12,ON);	

	gdh2_init(&ctx1,grp_id,N,&ctr_drbg); gdh2_init(&ctx2,grp_id,N,&ctr_drbg);

	if ((ret = gdh2_make_firstval(&ctx1,&olen,data_sent1,BUFLEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	sendDataIndividual(area2,line2,member2,data_sent1,olen,UART1);
	inizializzaKnxTpUart(area2,line2,member2);
	receiveData(data_rec2,olen);
	turnGPIO(PORTD,15,ON);

	if ((ret = gdh2_make_val(&ctx2,2,data_rec2,BUFLEN,&olen,data_sent2,BUFLEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	sendDataIndividual(area3,line3,member3,data_sent2,olen,UART6);
	setListenToBroadcasts(true);
	receiveData(data_rec1,(N-1)*LEN);
	turnGPIO(PORTD,13,ON);

	if ((ret = gdh2_compute_key(&ctx1,1,data_rec1,BUFLEN,&olen,data_sent1,BUFLEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	if ((ret = gdh2_compute_key(&ctx2,2,data_rec1,BUFLEN,&olen,data_sent2,BUFLEN,&ctr_drbg)) != 0) {
		turnGPIO(PORTD,14,ON);
    	entropy_free( &entropy );
		return(ret);
	}

	turnGPIO(PORTD,12,OFF);
	turnGPIO(PORTD,15,OFF);

/* // sanity check: verify if the first 2 nodes have computed the same key -- DISABLED
	for(ret = 0; ret < olen; ret++) {
		if (data_sent1[ret] != data_sent2[ret]) {
			turnGPIO(PORTD,14,ON);
			return -1;
		}
	}
*/

	entropy_free(&entropy);
	gdh2_free(&ctx1);
	gdh2_free(&ctx2);
	
	return 0;
}
