# include "bd1.h"
# include "polarssl/entropy.h"
# include "polarssl/ctr_drbg.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>

# define BUFDIM 66

long int n;
unsigned char **buf1,**buf2,**buf3;
sem_t sem1,sem2;
ecp_group_id grp_id = POLARSSL_ECP_DP_SECP256R1;

void *node(void *arg) {
	char *personalization = "Burmester-Desmedt1 Test";
	unsigned int olen;
	int id = (int) arg,i,prec,succ,index,ret;
	unsigned char *data;
	ctr_drbg_context ctr_drbg;
	entropy_context entropy;
	bd1_context ctx;
		
	entropy_init(&entropy);
    if (ctr_drbg_init(&ctr_drbg, entropy_func, &entropy,
                               (const unsigned char *) personalization,
                               strlen(personalization)) != 0) {
		printf("Failed in ctr_drbg_init\n");
    	exit(0);
	}

	if ((ret = bd1_init(&ctx,grp_id) != 0)) {
		printf("Node %d: init function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}

	if (n > 3)
		data = malloc((n-2)*BUFDIM*sizeof(unsigned char));
	else 
		data = malloc(2*BUFDIM*sizeof(unsigned char *));

	if ((ret = bd1_gen_point(&ctx,&olen,buf1[id],BUFDIM,&ctr_drbg)) != 0) {
		printf("Node %d: gen_point function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}
	printf("Node %d: z value computed correctly\n",id);
	sem_post(&sem1);
	do { sem_getvalue(&sem1,&ret); } while (ret != n);
	prec = (id-1) >= 0 ? (id-1) : (n-1);
	memcpy(data,buf1[prec],BUFDIM);
	succ = (id+1) % n;
	memcpy(data+BUFDIM,buf1[succ],BUFDIM);
	if ((ret = bd1_gen_value(&ctx,data,BUFDIM*2,&olen,buf2[id],BUFDIM,&ctr_drbg)) != 0) {
		printf("Node %d: gen_value function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}
	sem_post(&sem2);
	printf("Node %d: X value computed correctly\n",id);
	do { sem_getvalue(&sem2,&ret); } while (ret != n);
	for (i = 1; i <= n-2; i++) {
		index = (id+i) % n;
		memcpy(data+((i-1)*BUFDIM),buf2[index],BUFDIM);
	}
	if ((ret = bd1_compute_key(&ctx,data,BUFDIM*(n-2),n,&olen,buf3[id],BUFDIM,&ctr_drbg)) != 0) {
		printf("Node %d: compute key function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}
	printf("Node %d: key computed correctly\n",id);

	free(data);

	return NULL;
}	

void cleanup() {
	int i;
	
	for (i = 0; i < n; i++) { 
		free(buf1[i]);
		free(buf2[i]);
		free(buf3[i]);
	}
	free(buf1); 
	free(buf2);
	free(buf3);
	sem_destroy(&sem1);
	sem_destroy(&sem2);
}

int main(int argc,char *argv[]) {
	int i;
	pthread_t *id;

	if (argc != 2) {printf("Usage: ./bd1_test number_of_parties\n"); return -1;}
	
	n = strtol(argv[1],NULL,10);
	buf1 = (unsigned char **) malloc(n*sizeof(unsigned char *));
	buf2 = (unsigned char **) malloc(n*sizeof(unsigned char *));
	buf3 = (unsigned char **) malloc(n*sizeof(unsigned char *));
	for (i = 0; i < n; i++) { 
		buf1[i] = malloc(BUFDIM*sizeof(unsigned char)); 
		memset(buf1[i],0,BUFDIM); 
		buf2[i] = malloc(BUFDIM*sizeof(unsigned char)); 
		memset(buf2[i],0,BUFDIM);
		buf3[i] = malloc(BUFDIM*sizeof(unsigned char)); 
		memset(buf3[i],0,BUFDIM); 
	}
	id = (pthread_t *) malloc(n*sizeof(pthread_t));
	sem_init(&sem1,0,0);
	sem_init(&sem2,0,0);

	for (i = 0; i < n; i++) {
		pthread_create(&id[i],NULL,&node,(void *) i);
	}

	for (i = 0; i < n; i++) {
		pthread_join(id[i],NULL);
	}

	for (i = 1; i < n; i++) {
		if (memcmp(buf3[i],buf3[0],BUFDIM) != 0) {
			printf("Key not corresponding\n");
			cleanup();
			return -1;
		}
	}

	cleanup();
	printf("TEST COMPLETED WITH SUCCESS\n");
	return 0;
}

