# include "bd2.h"
# include "polarssl/entropy.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>

long int n;
unsigned int olen;
sem_t *sem;
unsigned char buf[150],key[150];
ctr_drbg_context ctr_drbg;
ecp_group_id id = POLARSSL_ECP_DP_SECP256R1;

void *chair(void *arg) {
	int ret,i;
	unsigned char pubvalue[66];
	bd2_context ctx;

	bd2_init(&ctx,id);

	if ((ret = bd2_make_public(&ctx,&olen,pubvalue,sizeof(pubvalue),&ctr_drbg)) != 0) {
		printf("Chair: failed generating dh keypair\n");
		return NULL;
	} else printf("Chair: dh keypair generated, %d bytes written\n",olen);

	if ((ret = bd2_make_key(&ctx,&ctr_drbg)) != 0) {
		printf("Chair: failed generating key\n");
		return NULL;
	} else printf("Chair: key generated\n");
	bd2_export_key(&ctx,&olen,key,sizeof(key));

	for (i = 1; i < n; i++) {
		memcpy(buf,pubvalue,sizeof(pubvalue));		

		sem_post(&sem[i]); sem_wait(&sem[0]);
		
		if ((ret = bd2_read_public(&ctx,buf,olen))) {
			printf("Chair: error reading peer's public value\n");
			return NULL;
		}
		
		if ((ret = bd2_make_val(&ctx,&olen,buf,sizeof(buf),&ctr_drbg)) != 0) {
			printf("Chair: failed generating value X\n");
			return NULL;
		} else printf("Chair: value X generated, %d bytes written\n",olen);

		sem_post(&sem[i]); sem_wait(&sem[0]);
	}

	arg = NULL;
	return arg;
}

void *node(void *arg) {
	int ret,i = (int) arg;
	unsigned int j;
	bd2_context ctx;

	bd2_init(&ctx,id);
	sem_wait(&sem[i]);

	if ((ret = bd2_read_public(&ctx,buf,sizeof(buf))) != 0) {
		printf("Node #%d: error %d reading peer's public value\n",i,ret);
		return NULL;
	}

	if ((ret = bd2_make_public(&ctx,&olen,buf,sizeof(buf),&ctr_drbg)) != 0) {
		printf("Node #%d: failed generating keypair\n",i);
		return NULL;
	} else printf("Node #%d: keypair generated, %d bytes written\n",i,olen);

	sem_post(&sem[0]); sem_wait(&sem[i]);
	
	bd2_import_val(&ctx,buf,olen);
	if ((ret = bd2_calc_key(&ctx,&ctr_drbg)) != 0)
	{
		printf("Node #%d: error computing key\n",i );
		return NULL;
	} else printf("Node #%d: key computed correctly\n",i);
	bd2_export_key(&ctx,&olen,buf,sizeof(buf));
	for (j = 0; j < olen; j++) {
		if (buf[j] != key[j]) {
			printf("Node #%d: ERROR! My key is not equal to the chair's key\n",i);
			sem_post(&sem[0]); 
			return NULL;
		}
	}
	printf("Node #%d: OK! My key is equal to the chair's key\n",i);
	sem_post(&sem[0]);
	return NULL;
}

int main(int argc,char *argv[]) {
	int i,ret;
	char *personalization = "my_app_specific_string";
	entropy_context entropy;
	pthread_t *id;

	if (argc != 2) {printf("Usage: ./test number_of_parties\n"); return -1;}
	
	n = strtol(argv[1],NULL,10);
	sem = (sem_t *) malloc(n*sizeof(sem_t));
	id = (pthread_t *) malloc(n*sizeof(pthread_t));

	entropy_init(&entropy);
    ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                               (const unsigned char *) personalization,
                               strlen(personalization));

	// error checking
	if (ret != 0)
    {
        	printf("Failed in ctr_drbg_init\n");
    		entropy_free(&entropy);
			return(ret);
    }
	
	pthread_create(&id[0],NULL,&chair,NULL);
	for (i = 1; i < n; i++) {
		pthread_create(&id[i],NULL,&node,(void *) i);
	}

	for (i = 0; i < n; i++) {
		pthread_join(id[i],NULL);
	}

	return 0;
}
