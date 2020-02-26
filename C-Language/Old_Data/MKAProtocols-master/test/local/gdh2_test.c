# include "gdh2.h"
# include "polarssl/entropy.h"
# include "polarssl/ctr_drbg.h"
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>

# define BUFDIM 66
# define KEYDIM 32

long int n;
unsigned char **buf;
sem_t *sem;
ecp_group_id grp_id = POLARSSL_ECP_DP_SECP256R1;

void print_buf(int id) {
	int j;

	for (j = 0; j < n*BUFDIM; j++) {
		if (j % (BUFDIM-1) == 0 && j != 0)
			printf("%d\n",buf[id][j]);
		else 
			printf("%d ",buf[id][j]);
	}
	printf("\n");
}

void *node(void *arg) {
	char *personalization = "GDH2 Test";
	unsigned int olen;
	int id = (int) arg,ret,i;
	gdh2_context ctx;
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

	if ((ret = gdh2_init(&ctx,grp_id,n,&ctr_drbg)) != 0) {
		printf("Node %d: init function fails returning %d! TEST FAILED!\n",id,ret);
		exit(0);
	}

	if (id == 0) {
		printf("Node %d: starting key exchange\n",id);
		if ((ret = gdh2_make_firstval(&ctx,&olen,buf[1],BUFDIM*n,&ctr_drbg)) != 0) {
			printf("Node %d: firstval function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		sem_post(&sem[1]);
		sem_wait(&sem[0]);
		if ((ret = gdh2_compute_key(&ctx,id+1,buf[id],BUFDIM*n,&olen,buf[id],
			BUFDIM*n,&ctr_drbg)) != 0) {
			printf("Node %d: compute_key function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		printf("Node %d: finished\n",id);
	}
	else if ((id > 0) && (id < n-1)) {
		sem_wait(&sem[id]);
		printf("Node %d: it's my turn...\n",id);
		if ((ret = gdh2_make_val(&ctx,id+1,buf[id],BUFDIM*n,&olen,buf[id+1],
			BUFDIM*n,&ctr_drbg)) != 0) {
			printf("Node %d: make_val function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		sem_post(&sem[id+1]);
		sem_wait(&sem[id]);
		if ((ret = gdh2_compute_key(&ctx,id+1,buf[id],BUFDIM*n,&olen,buf[id],
			BUFDIM*n,&ctr_drbg)) != 0) {
			printf("Node %d: compute_key function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		printf("Node %d: finished\n",id);
	}
	else {
		sem_wait(&sem[id]);
		printf("Node %d: last step\n",id);
		if ((ret = gdh2_make_val(&ctx,id+1,buf[id],BUFDIM*n,&olen,buf[0],
			BUFDIM*n,&ctr_drbg)) != 0) {
			printf("Node %d: make_val function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		for (i = 1; i < n-1; i++) {
			memcpy(buf[i],buf[0],BUFDIM*n);
			sem_post(&sem[i]);
		}
		sem_post(&sem[0]);
		if ((ret = gdh2_export_key(&ctx,&olen,buf[id],BUFDIM*n)) != 0) {
			printf("Node %d: export_key function fails returning %d! TEST FAILED!\n",id,ret);
			exit(0);
		}
		printf("Node %d: finished\n",id);
	}
	
	entropy_free(&entropy);
	return NULL;
}	

void cleanup() {
	int i;

	for (i = 0; i < n; i++) { 
		free(buf[i]);
		sem_destroy(&sem[i]);
	}	

	free(buf);
	free(sem);
}

int main(int argc,char *argv[]) {
	int i;
	pthread_t *id;

	if (argc != 2) {printf("Usage: ./gdh2_test number_of_parties\n"); return -1;}

	n = strtol(argv[1],NULL,10);
	buf =  malloc(n*sizeof(unsigned char *));
	sem =  malloc(n*sizeof(sem_t));
	for (i = 0; i < n; i++) { 
		buf[i] = malloc(BUFDIM*n*sizeof(unsigned char)); 
		memset(buf[i],0,BUFDIM*n);
		sem_init(&sem[i],0,0);
	}
	id =  malloc(n*sizeof(pthread_t));

	for (i = 0; i < n; i++) {
		pthread_create(&id[i],NULL,&node,(void *) i);
	}

	for (i = 0; i < n; i++) {
		pthread_join(id[i],NULL);
	}

	for (i = 1; i < n; i++) {
		if (memcmp(buf[i],buf[0],KEYDIM) != 0) {
			printf("Key not corresponding\n");
			cleanup();
			return -1;
		}
	}

	cleanup();
	printf("TEST COMPLETED WITH SUCCESS\n");
	return 0;
}
