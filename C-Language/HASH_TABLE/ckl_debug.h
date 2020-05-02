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
#if DEBUG_FLAG
#define DEBUG(msg,arg...) printf("%20s:%4d " msg"\n",__FILE__,__LINE__,##arg)
#define PRINT_FUNC(msg,arg...) printf("%20s:%4d Enter %s" msg"\n",__FILE__,__LINE__,__FUNCTION__,##arg)
#define DEBUG_COLOR(color,msg,arg...) printf("%s%20s:%4d " msg"%s\n",color,__FILE__,__LINE__,##arg,NONECOLOR)
#else
#define DEBUG(msg,arg...)
#define PRINT_FUNC(msg,arg...)
#define DEBUG_COLOR(color,msg,arg...)
#endif//DEBUG_FLAG

#define ERROR(msg,arg...) printf("\n\033[1;31m %20s:%4d " msg"\033[0m\n",__FILE__,__LINE__,##arg)

//define enum ret
typedef enum
{
	INITIALIZATION       = -1,    //< Iintialization
	SUCCESS              = 0,     //< Success
	METHOD_NOT_SUPPORTED = 9000,  //< Method not supported
	REQUEST_DENIED       = 9001,  //< Request denied (no reason specified)
	INTERNAL_ERROR       = 9002,  //< Internal error
	INVALID_ARGUMENTS    = 9003,  //< Invalid arguments
} Ret;

#endif //CKL_DEBUH_H
