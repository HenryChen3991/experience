# include "mka.h"
# include "polarssl/entropy.h"
# include "polarssl/ctr_drbg.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>

# define BUFDIM 66

long int n;
unsigned char **buf1,**buf2;
sem_t *write,*read;

ecp_group_id grp_id = POLARSSL_ECP_DP_SECP256R1;

void *node(void *arg) {
	char *personalization = "MKA Test";
	unsigned int olen;
	int id = (int) arg,i,j,index,ret;
	unsigned char *data;
	mka_context ctx;
	ctr_drbg_context ctr_drbg;
	entropy_context entropy;
		
	entropy_init(&entropy);
    if ((ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                               (const unsigned char *) personalization,
                               strlen(personalization))) != 0) {
        	printf("Failed in ctr_drbg_init\n");
    		entropy_free(&entropy);
			exit(0);
    }

	if ((ret = mka_init(&ctx,grp_id)) != 0) {
		printf("Node %d: init function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}

	if ((ret = mka_set_part(&ctx,n,&ctr_drbg)) != 0) {
		printf("Node %d: set_part function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}

	data = malloc(BUFDIM*(n-1)*sizeof(unsigned char));

	for (i = 0; i <= (n-2); i++) {
		printf("Node %d: computing broadcast value\n",id);
		if ((ret = mka_make_broadval(&ctx,&olen,buf1[id],BUFDIM,&ctr_drbg)) != 0) {
			printf("Node %d: make_broadval function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		for (j = 0; j < (n-1); j++) sem_post(&read[id]);
		index = 0;
		for (j = 0; j < n; j++) {
			if (j != id) {
				sem_wait(&read[j]);
				memcpy(data+(BUFDIM*index),buf1[j],BUFDIM);
				sem_post(&write[j]);
				index++;
			}
		}
		if ((ret = mka_acc_broadval(&ctx,data,BUFDIM*(n-1),&ctr_drbg)) != 0) {
			printf("Node %d: acc_broadval function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		printf("Node %d: broadcast values added\n",id);
		for (j = 0; j < (n-1); j++) sem_wait(&write[id]);
	}	
	
	printf("Node %d: computing shared key\n",id);
	if ((ret = mka_compute_key(&ctx,&olen,buf2[id],BUFDIM,&ctr_drbg)) != 0) {
			printf("Node %d: compute_key function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
	}

	entropy_free(&entropy);
	free(data);

	return NULL;
}	

void cleanup() {
	int i;

	for (i = 0; i < n; i++) { 
		free(buf1[i]);
		free(buf2[i]);
		sem_destroy(&read[i]);
		sem_destroy(&write[i]);
	}	

	free(buf1);
	free(buf2);
	free(read);
	free(write);
}

int main(int argc,char *argv[]) {
	int i;
	pthread_t *id;

	if (argc != 2) {printf("Usage: ./mka_test number_of_parties\n"); return -1;}

	n = strtol(argv[1],NULL,10);
	buf1 =  malloc(n*sizeof(unsigned char *));
	buf2 =  malloc(n*sizeof(unsigned char *));
	read = malloc(n*sizeof(sem_t));
	write = malloc(n*sizeof(sem_t));
	for (i = 0; i < n; i++) { 
		buf1[i] = malloc(BUFDIM*sizeof(unsigned char)); 
		memset(buf1[i],0,BUFDIM);
		buf2[i] = malloc(BUFDIM*sizeof(unsigned char));
		memset(buf2[i],0,BUFDIM);
		sem_init(&read[i],0,0);
		sem_init(&write[i],0,0);
	}
	id =  malloc(n*sizeof(pthread_t));

	for (i = 0; i < n; i++) {
		pthread_create(&id[i],NULL,&node,(void *) i);
	}

	for (i = 0; i < n; i++) {
		pthread_join(id[i],NULL);
	}

	for (i = 1; i < n; i++) {
		if (memcmp(buf2[i],buf2[0],BUFDIM) != 0) {
			printf("Key not corresponding\n");
			cleanup();
			return -1;
		}
	}

	cleanup();
	printf("TEST COMPLETED WITH SUCCESS\n");
	return 0;
}

