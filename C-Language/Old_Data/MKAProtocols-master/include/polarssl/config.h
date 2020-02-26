#define POLARSSL_AES_C
#define POLARSSL_ENTROPY_C
#define POLARSSL_CTR_DRBG_C
#define POLARSSL_SHA512_C
#define POLARSSL_BIGNUM_C
#define POLARSSL_ECP_C

#define POLARSSL_PLATFORM_C
#define POLARSSL_PLATFORM_MEMORY
#define POLARSSL_MEMORY_BUFFER_ALLOC_C
#define POLARSSL_PLATFORM_NO_STD_FUNCTIONS

#define POLARSSL_NO_DEFAULT_ENTROPY_SOURCES
#define POLARSSL_NO_PLATFORM_ENTROPY

# define POLARSSL_ECP_DP_NONE_ENABLED
# define POLARSSL_ECP_DP_SECP192R1_ENABLED  /*!< 192-bits NIST curve  */
# define POLARSSL_ECP_DP_SECP224R1_ENABLED      /*!< 224-bits NIST curve  */
# define POLARSSL_ECP_DP_SECP256R1_ENABLED      /*!< 256-bits NIST curve  */
# define POLARSSL_ECP_DP_SECP384R1_ENABLED      /*!< 384-bits NIST curve  */
# define POLARSSL_ECP_DP_SECP521R1_ENABLED    /*!< 521-bits NIST curve  */
# define POLARSSL_ECP_DP_BP256R1_ENABLED      /*!< 256-bits Brainpool curve */
# define POLARSSL_ECP_DP_BP384R1_ENABLED   /*!< 384-bits Brainpool curve */
# define POLARSSL_ECP_DP_BP512R1_ENABLED  /*!< 512-bits Brainpool curve */
# define POLARSSL_ECP_DP_M255_ENABLED       /*!< Curve25519               */
# define POLARSSL_ECP_DP_SECP192K1_ENABLED     /*!< 192-bits "Koblitz" curve */
# define POLARSSL_ECP_DP_SECP224K1_ENABLED      /*!< 224-bits "Koblitz" curve */
# define POLARSSL_ECP_DP_SECP256K1_ENABLED     /*!< 256-bits "Koblitz" curve */

#define POLARSSL_ECP_MAX_BITS             521 /**< Maximum bit size of groups */
#define POLARSSL_ECP_WINDOW_SIZE            6 /**< Maximum window size used */
#define POLARSSL_ECP_FIXED_POINT_OPTIM      1 /**< Enable fixed-point speed-up */
