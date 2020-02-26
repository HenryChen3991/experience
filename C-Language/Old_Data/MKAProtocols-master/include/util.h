# include "polarssl/ecp.h"
# include "polarssl/bignum.h"
# include "polarssl/ctr_drbg.h"

# include "stm32f4d_helper.h"

# define BAD_INPUT_DATA            -1
# define BUFFER_TOO_SHORT		   -2

int ecp_opp(const ecp_group *grp,ecp_point *R,const ecp_point *P);
int test_ecp_mul(ecp_group *grp, ecp_point *R,
                const mpi *m, const ecp_point *P,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng );
