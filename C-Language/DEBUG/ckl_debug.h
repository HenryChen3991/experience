#ifndef CKL_DEBUG_H
#define CKL_DEBUG_H

//define color
#define NONECOLOR "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

//define debug macro
#define DEBUG_FLAG 1
#define DBGLEVEL_MSG_ERROR 1
#define DBGLEVEL_MSG_NOTICE 1
#if DEBUG_FLAG
#define DEBUG(msg,arg...) printf("%20s:%4d " msg"\n",__FILE__,__LINE__,##arg)
#define PRINT_FUNC(msg,arg...) printf("%20s:%4d Enter %s" msg"\n",__FILE__,__LINE__,__FUNCTION__,##arg)
#define DEBUG_COLOR(color,msg,arg...) printf("%s%20s:%4d " msg"%s\n",color,__FILE__,__LINE__,##arg,NONECOLOR)
#define SET_COLOR(color) printf("%s",color);

#define ERROR(msg,arg...) { \
    if (DBGLEVEL_MSG_ERROR) { \
        printf("\n\033[1;31m%20s:%4d " msg"\033[0m\n",__FILE__,__LINE__,##arg); \
    } \
}

#define NOTICE(msg,arg...) { \
    if (DBGLEVEL_MSG_NOTICE) { \
        printf("\n\033[1;32m%20s:%4d " msg"\033[0m\n",__FILE__,__LINE__,##arg); \
    } \
}

#else
#define DEBUG(msg,arg...)
#define PRINT_FUNC(msg,arg...)
#define DEBUG_COLOR(color,msg,arg...)
#define SET_COLOT(color)
#define ERROR(msg,arg...)
#define NOTICE(msg,arg...)
#endif//DEBUG_FLAG


//define enum ret
typedef enum
{
	INITIALIZATION       = -1,    //< Iintialization
	CMSRET_SUCCESS       = 0,     //< Success
	METHOD_NOT_SUPPORTED = 9000,  //< Method not supported
	REQUEST_DENIED       = 9001,  //< Request denied (no reason specified)
	INTERNAL_ERROR       = 9002,  //< Internal error
	INVALID_ARGUMENTS    = 9003,  //< Invalid arguments
	NOT_FOUND            = 9004,  //< Not Found
} Ret;

/**
 * This is common used string length types.
 */
#define BUFLEN_4        4     //!< buffer length 4
#define BUFLEN_8        8     //!< buffer length 8
#define BUFLEN_16       16    //!< buffer length 16
#define BUFLEN_18       18    //!< buffer length 18 -- for ppp session id
#define BUFLEN_24       24    //!< buffer length 24 -- mostly for password
#define BUFLEN_32       32    //!< buffer length 32
#define BUFLEN_40       40    //!< buffer length 40
#define BUFLEN_48       48    //!< buffer length 48
#define BUFLEN_64       64    //!< buffer length 64
#define BUFLEN_80       80    //!< buffer length 80
#define BUFLEN_128      128   //!< buffer length 128
#define BUFLEN_256      256   //!< buffer length 256
#define BUFLEN_264      264   //!< buffer length 264
#define BUFLEN_512      512   //!< buffer length 512
#define BUFLEN_1024     1024  //!< buffer length 1024

#define MAC_ADDR_LEN    6     //!< Mac address len in an array of 6 bytes
#define MAC_STR_LEN     17    //!< Mac String len with ":". eg: xx:xx:xx:xx:xx:xx


#endif //CKL_DEBUH_H
