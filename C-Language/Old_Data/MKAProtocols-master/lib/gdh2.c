# include "gdh2.h"

int gdh2_init(gdh2_context *ctx,ecp_group_id id,int N,void *p_rng) {
	int ret;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_group_init(&ctx->grp);
	mpi_init(&ctx->priv);
	mpi_init(&ctx->key);
	MPI_CHK(ecp_use_known_dp(&ctx->grp,id));
	MPI_CHK(ecp_gen_privkey(&ctx->grp,&ctx->priv,ctr_drbg_random,p_rng));
	ctx->N = N;

cleanup:
	return ret;
}

int gdh2_make_firstval(gdh2_context *ctx,unsigned int *olen,unsigned char *obuf,
                       unsigned int obuflen,void *p_rng) {
	int ret;
	ecp_point p;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_point_init(&p);
	MPI_CHK(test_ecp_mul(&ctx->grp,&p,&ctx->priv,&ctx->grp.G,ctr_drbg_random,p_rng));	
	MPI_CHK(ecp_tls_write_point(&ctx->grp,&p,0,olen,obuf,obuflen));

cleanup:
	ecp_point_free(&p);
	return ret;
}

int gdh2_make_val(gdh2_context *ctx,int index,unsigned char *buf,unsigned int buflen,
                  unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng) {
	int ret,i,term;
	unsigned int len = 0;
	const unsigned char *p = buf;
	ecp_point read,write;

	if (ctx == NULL)
		return BAD_INPUT_DATA;

	ecp_point_init(&read);
	ecp_point_init(&write);

	// processing all the points
	if (index == ctx->N) {
		// if I'm the last node, compute the key
		MPI_CHK(ecp_tls_read_point(&ctx->grp,&read,&p,buflen));
		MPI_CHK(test_ecp_mul(&ctx->grp,&write,&ctx->priv,&read,ctr_drbg_random,p_rng));
		MPI_CHK(mpi_copy(&ctx->key,&write.X));
		i = 1;
	} 
	else if ((index >= 2) && (index < ctx->N)) {
		i = 0;
	}
	else 
		return BAD_INPUT_DATA;

	term = (index == 2) ? 1 : index;
	for( ; i < term; i++) {
		MPI_CHK(ecp_tls_read_point(&ctx->grp,&read,&p,buflen));
		MPI_CHK(test_ecp_mul(&ctx->grp,&write,&ctx->priv,&read,ctr_drbg_random,p_rng));
		MPI_CHK(ecp_tls_write_point(&ctx->grp,&write,0,olen,obuf+len,obuflen-len));
		len += *olen;
		if (i == 0) {
			// add the old cardinal value
			MPI_CHK(ecp_tls_write_point(&ctx->grp,&read,0,olen,obuf+len,obuflen-len));
			len += *olen;
		}
	}
	// add an extra point if I'm the second node 
	if (index == 2) {
		MPI_CHK(test_ecp_mul(&ctx->grp,&write,&ctx->priv,&ctx->grp.G,ctr_drbg_random,p_rng));
		MPI_CHK(ecp_tls_write_point(&ctx->grp,&write,0,olen,obuf+len,obuflen-len));
		len += *olen;
	}
	*olen = len;

cleanup:
	ecp_point_free(&read);
	ecp_point_free(&write);	
	return ret;
}

int gdh2_compute_key(gdh2_context *ctx,int index,unsigned char *buf,unsigned int buflen,
                     unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng) {
	int ret,plen;
	const unsigned char *p;
	ecp_point read,tmp;

	if ((ctx == NULL) || (index < 1) || (index >= ctx->N))
		return BAD_INPUT_DATA;

	ecp_point_init(&read);
	ecp_point_init(&tmp);

	plen = 2*mpi_size(&ctx->grp.P) + 2;
	p = buf + (plen*(ctx->N-index-1));

	MPI_CHK(ecp_tls_read_point(&ctx->grp,&read,&p,buflen));
	MPI_CHK(test_ecp_mul(&ctx->grp,&tmp,&ctx->priv,&read,ctr_drbg_random,p_rng));
	MPI_CHK(mpi_copy(&ctx->key,&tmp.X));

	if(mpi_size(&ctx->key) > obuflen )
        return BUFFER_TOO_SHORT;

    *olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0 );
    MPI_CHK(mpi_write_binary(&ctx->key,obuf,*olen));

cleanup:
	ecp_point_free(&read);
	ecp_point_free(&tmp);
	return ret;
}

int gdh2_export_key(gdh2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen) {
	if (ctx == NULL)
		return BAD_INPUT_DATA;

	if(mpi_size(&ctx->key) > buflen )
        return BUFFER_TOO_SHORT;

    *olen = ctx->grp.pbits / 8 + ((ctx->grp.pbits % 8) != 0 );
    return mpi_write_binary(&ctx->key,buf,*olen);
}

int gdh2_free(gdh2_context *ctx) {
	if (ctx == NULL)
		return BAD_INPUT_DATA;
	
	ecp_group_free(&ctx->grp);
	mpi_free(&ctx->priv);
	mpi_free(&ctx->key);
	return 0;
}
