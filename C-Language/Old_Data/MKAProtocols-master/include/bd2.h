/** 
* \file bd2.h
*
* \brief Burmester-Desmdet key exchange
*
* Refer to: Burmester, M., Desmedt, Y.: Efficient and Secure Conference Key Distribution
*/

# include "util.h"

/**
* \brief BD2 context structure
*/
typedef struct {
    ecp_group grp;        /** ecp group */
	mpi priv;             /** private exponent */
	ecp_point pub;        /** public value */
	ecp_point peer;       /** peer's public value */
	ecp_point X;          /** encrypted key (unused for the root)*/
    ecp_point key;        /** key value */
} bd2_context;

/**
* \brief BD2 initialization function
*
* \param ctx   BD2 context to be initialized
* \param id    id of the employed elliptic curve
* \param p_rng initialization value of rng
*
* \return    0 if successful
*/
int bd2_init(bd2_context *ctx,ecp_group_id id);

/**
* \brief BD2 make key function
*
* \param ctx    BD2 context to be initialized
* \param p_rng  initialization value for rng
*
* \return    0 if successful
*
* \note This function is used by the root node to compute the key K that will be
*       propagated to all the other nodes. Since this is an asymmetric protocol
*       this function is meant to be used by only one node (the root node, refer
*       to the paper for more details).
*/
int bd2_make_key(bd2_context *ctx,void *p_rng);

/**
* \brief BD2 export key value
*
* \param ctx     BD2 context to be initialized
* \param olen    number of bytes written on the output buffer
* \param buf     output buffer
* \param buflen  length of the output buffer
*
* \return    0 if successful
*/
int bd2_export_key(bd2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen);

/**
* \brief BD2 make value function
*
* \param ctx     BD2 context to be initialized
* \param olen    number of bytes written on the output buffer
* \param buf     output buffer
* \param buflen  length of the output buffer
* \param p_rng   initialization value for rng
*
* \return    0 if successful
*
* \note This function computes and exports the encrypted key to a generic node.
*/
int bd2_make_val(bd2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen,void *p_rng);

/**
* \brief BD2 make value function
*
* \param ctx     BD2 context to be initialized
* \param buf     input buffer
* \param buflen  length of the input buffer
*
* \return    0 if successful
*
* \note This function imports the encrypted key.
*/
int bd2_import_val(bd2_context *ctx,unsigned char *buf,unsigned int buflen);

/**
* \brief BD2 compute key function
*
* \param ctx     BD2 context to be initialized
* \param p_rng   initialization value for rng
*
* \return    0 if successful
*/
int bd2_calc_key(bd2_context *ctx,void *p_rng);

/**
* \brief BD2 context free function
*
* \param ctx     BD2 context to be initialized
*
* \return    0 if successful
*/
int bd2_free(bd2_context *ctx);

/**
* \brief BD2 make public function
*
* \param ctx     BD2 context to be initialized
* \param olen    number of bytes written on the output buffer
* \param buf     output buffer
* \param buflen  length of the output buffer
* \param p_rng   initialization value for rng
*
* \return    0 if successful
*
*/
int bd2_make_public(bd2_context *ctx,unsigned int *olen,unsigned char *buf,
			       unsigned int buflen,void *p_rng);

/**
* \brief BD2 read public function
*
* \param ctx     BD2 context to be initialized
* \param buf     input buffer
* \param buflen  length of the input buffer
*
* \return    0 if successful
*
*/
int bd2_read_public(bd2_context *ctx,const unsigned char *buf,unsigned int buflen);
