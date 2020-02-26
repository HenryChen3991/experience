/** 
* \file mka.h
*
* \brief Multiparty Key Exchange
*
* Refer to: Antonini, A., Barenghi, A., Pelosi, G.: Security Analysis of Building Automation Networks
*                                                   Threat Model and Viable Mitigation Techniques 
*/

# include "util.h"

# define MAX_ROUND_NUMBER_EXCEEDED -3
# define ROUND_NOT_COMPLETED	   -4

/**
* \brief MKA context structure
*/
typedef struct {
	ecp_group grp; /** ecp group */
	mpi a;         /** private exponent */
	ecp_point k;   /** accumulator */
	mpi key;       /** key value */
	int n;         /** number of parties */
	int r;         /** current round */
} mka_context;

/**
* \brief MKA initialization function
*
* \param ctx MKA context to be initialized
* \param id  id of the employed elliptic curve 
*
* \return    0 if successful
*/
int mka_init(mka_context *ctx,ecp_group_id id);

/**
* \brief MKA set number of parties 
*
* \param ctx    MKA context     
* \param n      number of parties
* \param p_rng  initialization value for rng
*
* \return 0 if successful
*/
int mka_set_part(mka_context *ctx,int n,void *p_rng);

/**
* \brief MKA generate broadcast value
*
* \param ctx    MKA context     
* \param olen   number of bytes written
* \param buf    output buffer
* \param buflen length of output buffer
* \param p_rng  initialization value for rng
*
* \return 0 if successful
*/
int mka_make_broadval(mka_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen,void *p_rng);

/**
* \brief MKA accumulate received broadcast values
*
* \param ctx    MKA context     
* \param buf    input buffer
* \param buflen length of input buffer
* \param p_rng  initialization value for rng
*
* \return 0 if successful
*/
int mka_acc_broadval(mka_context *ctx,unsigned char *buf,unsigned int buflen,void *p_rng);

/**
* \brief MKA compute key
*
* \param ctx    MKA context     
* \param olen   number of bytes written
* \param buf    output buffer
* \param buflen length of output buffer
* \param p_rng  initialization value for rng
*
* \return 0 if successful
*/
int mka_compute_key(mka_context *ctx,unsigned int *olen,unsigned char *buf,unsigned char buflen,void *p_rng);

/**
* \brief MKA context free function
*
* \param ctx    MKA context     
*
* \return 0 if successful
*/
int mka_free(mka_context *ctx);
