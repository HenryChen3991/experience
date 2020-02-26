# include "util.h"

int ecp_opp( const ecp_group *grp, ecp_point *R, const ecp_point *P )
{
    int ret;

    /* Copy */
    if( R != P )
        MPI_CHK( ecp_copy( R, P ) );

    /* In-place opposite */
    if( mpi_cmp_int( &R->Y, 0 ) != 0 )
        MPI_CHK( mpi_sub_mpi( &R->Y, &grp->P, &R->Y ) );

cleanup:
    return( ret );
}

int test_ecp_mul(ecp_group *grp, ecp_point *R,
                const mpi *m, const ecp_point *P,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
	int ret;

	turnGPIO(PORTB,11,ON);	
	ret = ecp_mul(grp,R,m,P,f_rng,p_rng);
	turnGPIO(PORTB,11,OFF);

	return ret;
}
