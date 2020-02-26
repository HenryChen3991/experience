# include "bd2.h"

// context initialization
int bd2_init(bd2_context *ctx,ecp_group_id id) {
	int ret;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

    ecp_group_init(&ctx->grp);
 	mpi_init(&ctx->priv);
	ecp_point_init(&ctx->pub);
	ecp_point_init(&ctx->peer);
	ecp_point_init(&ctx->X);
    ecp_point_init(&ctx->key);
	MPI_CHK(ecp_use_known_dp(&ctx->grp,id));

cleanup:
	return ret;
}

/*
	Function to generate the key K, a simple wrapper around bd2_make_key
	It should be used only by the node deciding the key that will be propagated
*/
int bd2_make_key(bd2_context *ctx,void *p_rng) {
    int ret;
    mpi d;
    
	if (ctx == NULL)
		return BAD_INPUT_DATA;

	mpi_init(&d);

    MPI_CHK(ecp_gen_keypair(&ctx->grp,&d,&ctx->key,ctr_drbg_random,p_rng));
    
cleanup:
	mpi_free(&d);	
	return ret;
}

/*
	Function for exporting the key value on a buffer
*/
int bd2_export_key(bd2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen) {
	if (mpi_size(&ctx->key.X) > buflen)
        return BUFFER_TOO_SHORT;

    *olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0);
    return mpi_write_binary(&ctx->key.X,buf,*olen);
}

// helper function 
int bd2_compute_shared_point(bd2_context *ctx,ecp_point *P,void *p_rng) {
	int ret;

	 /*
     * Make sure Q is a valid pubkey before using it
     */
    MPI_CHK(ecp_check_pubkey(&ctx->grp,&ctx->peer));

	// computing the shared key as the product of our private exponent for the 
    // public value of the other node
    MPI_CHK(test_ecp_mul(&ctx->grp,P,&ctx->priv,&ctx->peer,ctr_drbg_random,p_rng));

	// check if the shared key is 0
    if(ecp_is_zero(P))
    {
        ret = POLARSSL_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

cleanup:
	return ret;	
}


/*
	Function generating the value sent by node i to an adjacent node j 
	to allow it to derive the key: X = K + (Kij^-1)
*/
int bd2_make_val(bd2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen,void *p_rng) {
	int ret;
    ecp_point P,oppP;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

    ecp_point_init(&P);
	ecp_point_init(&oppP);

	// first we compute Kij, the shared key between node i and j
	MPI_CHK(bd2_compute_shared_point(ctx,&P,p_rng));
		
	// computing X = key + (-P)
	ecp_opp(&ctx->grp,&oppP,&P);
	ecp_add(&ctx->grp,&ctx->X,&ctx->key,&oppP);

	// exporting on buffer
	MPI_CHK(ecp_tls_write_point(&ctx->grp,&ctx->X,0,olen,buf,buflen));
    
cleanup:	 
    ecp_point_free(&P);
	ecp_point_free(&oppP);
	return(ret);
}

/*	
	Complementary to bd2_make_val. Import the value sent by adjacent node
*/
int bd2_import_val(bd2_context *ctx,unsigned char *buf,unsigned int buflen) {
    int ret;
	const unsigned char *p = buf;
 
	if (ctx == NULL)
		return BAD_INPUT_DATA;

	if ((ret = ecp_tls_read_point(&ctx->grp,&ctx->X,&p,buflen)) != 0)
		return ret;
	
	return 0;
}

/*
	Function to derive the key as: K = X + Kij 
*/
int bd2_calc_key(bd2_context *ctx,void *p_rng) {
	int ret;
    ecp_point P;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

    ecp_point_init(&P);

	MPI_CHK(bd2_compute_shared_point(ctx,&P,p_rng));
	MPI_CHK(ecp_add(&ctx->grp,&ctx->key,&ctx->X,&P));

cleanup:
	ecp_point_free(&P);
	return ret;
}

int bd2_free(bd2_context *ctx) {
	if (ctx == NULL)
		return BAD_INPUT_DATA;
	
	ecp_group_free(&ctx->grp);
	mpi_free(&ctx->priv);
	ecp_point_free(&ctx->pub);
	ecp_point_free(&ctx->peer);
	ecp_point_free(&ctx->X);
	ecp_point_free(&ctx->key);

	return 0;
}

int bd2_make_public(bd2_context *ctx,unsigned int *olen,unsigned char *buf,
			       unsigned int buflen,void *p_rng) {
	int ret;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	MPI_CHK(ecp_gen_keypair(&ctx->grp,&ctx->priv,&ctx->pub,ctr_drbg_random,p_rng));
	MPI_CHK(ecp_tls_write_point(&ctx->grp,&ctx->pub,0,olen,buf,buflen));
	return ret; 

cleanup:
	return ret;
}

int bd2_read_public(bd2_context *ctx,const unsigned char *buf,unsigned int buflen) {
	int ret;
	const unsigned char *p = buf;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	if ((ret = ecp_tls_read_point(&ctx->grp,&ctx->peer,&p,buflen)) != 0)
		return ret; 
	
	return 0;		
}
