/** 
* \file gdh2.h
*
* \brief Generalized Diffie-Hellman 2
*
* Refer to: M. Steiner et al.: Diffie-Hellman Key Distribution Extended to Group Communication 
*/

# include "util.h"

/**
* \brief GDH2 context structure
*/
typedef struct {
	ecp_group grp; /** ecp group */
	mpi priv;	   /** private ephemeral key */
	mpi key;	   /** key value */
	int N;	       /** number of partecipants */
} gdh2_context;

/**
* \brief GDH2 initialization function
*
* \param ctx GDH2 context to be initialized
* \param id  id of the employed elliptic curve 
*
* \return    0 if successful
*/
int gdh2_init(gdh2_context *ctx,ecp_group_id id,int N,void *p_rng);

/**
* \brief GDH2 compute first value (first node of the chain)
*
* \param ctx     GDH2 context to be initialized
* \param olen    number of bytes written on the output buffer
* \param obuf    output buffer
* \param obuflen length of the output buffer
* \p_rng         initialization value for rng
*
* \return    0 if successful
*/
int gdh2_make_firstval(gdh2_context *ctx,unsigned int *olen,unsigned char *obuf,
                       unsigned int obuflen,void *p_rng);

/**
* \brief GDH2 compute value (generic node of the chain (exept 1st))
*
* \param ctx     GDH2 context to be initialized
* \param index   index of the node (from 1 to N)
* \param buf     input buffer
* \param buflen  length of input buffer
* \param olen    number of bytes written on the output buffer
* \param obuf    output buffer
* \param obuflen length of the output buffer
* \p_rng         initialization value for rng
*
* \return    0 if successful
*/
int gdh2_make_val(gdh2_context *ctx,int index,unsigned char *buf,unsigned int buflen,
                  unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng);

/**
* \brief GDH2 compute the key (generic node of the chain (exept the last))
*
* \param ctx     GDH2 context to be initialized
* \param index   index of the node (from 1 to N)
* \param buf     input buffer
* \param buflen  length of input buffer
* \param olen    number of bytes written on the output buffer
* \param obuf    output buffer
* \param obuflen length of the output buffer
* \p_rng         initialization value for rng
*
* \return    0 if successful
*/
int gdh2_compute_key(gdh2_context *ctx,int index,unsigned char *buf,unsigned int buflen,
                     unsigned int *olen,unsigned char *obuf,unsigned int obuflen,void *p_rng);

/**
* \brief GDH2 export key value
*
* \param ctx     GDH2 context to be initialized
* \param olen    number of bytes written on the output buffer
* \param buf     output buffer
* \param buflen  length of the output buffer
*
* \return    0 if successful
*/
int gdh2_export_key(gdh2_context *ctx,unsigned int *olen,unsigned char *buf,unsigned int buflen);

/**
* \brief GDH2 context free function
*
* \param ctx     GDH2 context to be initialized
*
* \return    0 if successful
*/
int gdh2_free(gdh2_context *ctx);
