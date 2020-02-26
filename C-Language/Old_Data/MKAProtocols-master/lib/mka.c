# include "mka.h"

int mka_init(mka_context *ctx,ecp_group_id id) {
	
	if (ctx == NULL) 
		return BAD_INPUT_DATA;

	ecp_group_init(&ctx->grp);
	mpi_init(&ctx->a);
	ecp_point_init(&ctx->k);
	mpi_init(&ctx->key);
	return (ecp_use_known_dp(&ctx->grp,id));
}

int mka_set_part(mka_context *ctx,int n,void *p_rng) {
	int ret;
	
	if ((ctx == NULL) || (n < 2))
		return BAD_INPUT_DATA;
	
	ctx->r = 0;
	ctx->n = n;
	MPI_CHK(ecp_gen_keypair(&ctx->grp,&ctx->a,&ctx->k,ctr_drbg_random,p_rng));

cleanup:
	return ret;	
}

int mka_make_broadval(mka_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen,void *p_rng) {
	int ret;
	ecp_point tmp;

	ecp_point_init(&tmp);	

	if (ctx == NULL)
		return BAD_INPUT_DATA;
	if (ctx->r > (ctx->n)-2)
		return MAX_ROUND_NUMBER_EXCEEDED;
	
	if (ctx->r == 0) {
		MPI_CHK(ecp_tls_write_point(&ctx->grp,&ctx->k,0,olen,buf,buflen));
	}
	else {
		MPI_CHK(test_ecp_mul(&ctx->grp,&tmp,&ctx->a,&ctx->k,ctr_drbg_random,p_rng));
		MPI_CHK(ecp_copy(&ctx->k,&tmp));
		MPI_CHK(ecp_tls_write_point(&ctx->grp,&ctx->k,0,olen,buf,buflen));
	}

cleanup:
	ecp_point_free(&tmp);
	return ret;
}

int mka_acc_broadval(mka_context *ctx,unsigned char *buf,unsigned int buflen,void *p_rng) {
	int ret,i;
	const unsigned char *p = buf;
	ecp_point sum,tmp1,tmp2;
	mpi r,exp;

	if (ctx == NULL)
		return BAD_INPUT_DATA;
	if (ctx->r > (ctx->n)-2)
		return MAX_ROUND_NUMBER_EXCEEDED;

	ecp_point_init(&tmp1);
	ecp_point_init(&tmp2);
	ecp_point_init(&sum);
	mpi_init(&r);
	mpi_init(&exp);

	ecp_set_zero(&sum);
	for (i = 0; i < (ctx->n)-1; i++) {
		MPI_CHK(ecp_tls_read_point(&ctx->grp,&tmp1,&p,buflen));
		MPI_CHK(ecp_add(&ctx->grp,&tmp2,&sum,&tmp1));
		MPI_CHK(ecp_copy(&sum,&tmp2));
	}

	if (ctx->r > 0) {
		MPI_CHK(ecp_opp(&ctx->grp,&tmp1,&ctx->k));
		MPI_CHK(mpi_lset(&r,ctx->r));
		MPI_CHK(test_ecp_mul(&ctx->grp,&tmp2,&r,&tmp1,ctr_drbg_random,p_rng));
		MPI_CHK(ecp_add(&ctx->grp,&tmp1,&sum,&tmp2));
	}
	else {
		MPI_CHK(ecp_copy(&tmp1,&sum));
	}
	MPI_CHK(mpi_lset(&r,(ctx->r)+1));
	MPI_CHK(mpi_inv_mod(&exp,&r,&(ctx->grp).N));
	MPI_CHK(test_ecp_mul(&ctx->grp,&ctx->k,&exp,&tmp1,ctr_drbg_random,p_rng));
	if (ctx->r < (ctx->n-2)) (ctx->r)++;
	
cleanup:
	ecp_point_free(&tmp1);
	ecp_point_free(&tmp2);
	ecp_point_free(&sum);
	mpi_free(&r);
	mpi_free(&exp);
	return ret;
}

int mka_compute_key(mka_context *ctx,unsigned int *olen,unsigned char *buf,unsigned char buflen,void *p_rng) {
	int ret;
	ecp_point tmp;

	ecp_point_init(&tmp);

	if (ctx == NULL)
		return BAD_INPUT_DATA;
	if (ctx->r > (ctx->n)-2)
		return MAX_ROUND_NUMBER_EXCEEDED;
	if (ctx->r < (ctx->n)-2)
		return ROUND_NOT_COMPLETED;

	MPI_CHK(test_ecp_mul(&ctx->grp,&tmp,&ctx->a,&ctx->k,ctr_drbg_random,p_rng));
	MPI_CHK(mpi_copy(&ctx->key,&tmp.X));

	if(mpi_size(&ctx->key) > buflen )
        return BUFFER_TOO_SHORT;

    *olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0 );
    return mpi_write_binary(&ctx->key,buf,*olen);

cleanup:
	ecp_point_free(&tmp);	
	return ret;
}

int mka_free(mka_context *ctx) {
	
	if (ctx == NULL)
		return BAD_INPUT_DATA;
	
	ecp_group_free(&ctx->grp);
	mpi_free(&ctx->a);
	ecp_point_free(&ctx->k);
	mpi_free(&ctx->key);
	return 0;
}
