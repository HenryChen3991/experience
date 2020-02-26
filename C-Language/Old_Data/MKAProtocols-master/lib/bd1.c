# include "bd1.h"

int bd1_init(bd1_context *ctx,ecp_group_id id) {
	if (ctx == NULL)
		return BAD_INPUT_DATA;

	mpi_init(&ctx->r);
	ecp_point_init(&ctx->z);
	ecp_point_init(&ctx->X);
	ecp_group_init(&ctx->grp);
	return (ecp_use_known_dp(&ctx->grp,id));
}

int bd1_gen_point(bd1_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen,void *p_rng) {
	int ret;
	ecp_point z;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_point_init(&z);
	MPI_CHK(ecp_gen_keypair(&ctx->grp,&ctx->r,&z,ctr_drbg_random,p_rng));
    MPI_CHK(ecp_tls_write_point(&ctx->grp,&z,0,
                                     olen,buf,buflen));
cleanup:
	ecp_point_free(&z);	
	return ret;
}

int bd1_gen_value(bd1_context *ctx,unsigned char *buf,unsigned int buflen,unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng) {
	int ret;
	const unsigned char *p = buf;
	ecp_point z1,z2;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_point_init(&z1);
	ecp_point_init(&z2);
	MPI_CHK(ecp_tls_read_point(&ctx->grp,&ctx->z,&p,buflen));
	MPI_CHK(ecp_tls_read_point(&ctx->grp,&z1,&p,buflen));
	MPI_CHK(ecp_sub(&ctx->grp,&z2,&z1,&ctx->z));
	if (ecp_is_zero(&z2)) 
		MPI_CHK(ecp_set_zero(&ctx->X));
	else 
		MPI_CHK(test_ecp_mul(&ctx->grp,&ctx->X,&ctx->r,&z2,ctr_drbg_random,p_rng));
	MPI_CHK(ecp_tls_write_point(&ctx->grp,&ctx->X,0,
                                     olen,obuf,obuflen));
cleanup:
	ecp_point_free(&z1);
	ecp_point_free(&z2);
	return ret;
}

int bd1_compute_key(bd1_context *ctx,unsigned char *buf,unsigned int buflen,int n,unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng) {
	int i,ret;
	const unsigned char *p = buf;
	ecp_point key,helper1,helper2;
	mpi exp1,exp2,k;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_point_init(&key);
	ecp_point_init(&helper1);
	ecp_point_init(&helper2);
	mpi_init(&exp1);
	mpi_init(&exp2);
	mpi_init(&k);

	MPI_CHK(mpi_mul_int(&exp1,&ctx->r,n));
	MPI_CHK(mpi_mod_mpi(&exp2,&exp1,&(ctx->grp.N)));
	if (mpi_cmp_int(&exp2,0) == 0)
		ecp_set_zero(&helper1);
	else {
		MPI_CHK(test_ecp_mul(&ctx->grp,&helper1,&exp2,&ctx->z,ctr_drbg_random,p_rng));
	}

	if (n == 2) 
		ecp_copy(&key,&helper1);
	else {
		MPI_CHK(mpi_lset(&exp1,n-1));
		MPI_CHK(test_ecp_mul(&ctx->grp,&helper2,&exp1,&ctx->X,ctr_drbg_random,p_rng));
		MPI_CHK(ecp_add(&ctx->grp,&key,&helper1,&helper2));

		for (i = 2; i <= n-1; i++) {
			MPI_CHK(mpi_lset(&exp1,n-i));
			MPI_CHK(ecp_tls_read_point(&ctx->grp,&helper1,&p,buflen));
			MPI_CHK(test_ecp_mul(&ctx->grp,&helper2,&exp1,&helper1,ctr_drbg_random,p_rng));
			MPI_CHK(ecp_add(&ctx->grp,&helper1,&key,&helper2));
			MPI_CHK(ecp_copy(&key,&helper1));
		}
	}

	MPI_CHK(mpi_copy(&k,&key.X));

	if(mpi_size(&k) > obuflen )
        return -1;

    *olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0 );
    return mpi_write_binary(&k,obuf,*olen);
	
cleanup:
	ecp_point_free(&key);
	ecp_point_free(&helper1);
	ecp_point_free(&helper2);
	mpi_free(&exp1);
	mpi_free(&exp2);
	mpi_free(&k);
	return ret;			
}

int bd1_free(bd1_context *ctx) {
	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_group_free(&ctx->grp);
	mpi_free(&ctx->r);
	ecp_point_free(&ctx->z);
	ecp_point_free(&ctx->X);

	return 0;	
}
