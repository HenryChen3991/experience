#define DEBUG_FLAG 1
#define DEBUG(msg,arg...) printf("%s:%s:%d " msg"\n",__FILE__,__FUNCTION__,__LINE__,##arg)

//color define
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

#define DEBUG_COLOR(color,msg,arg...) { \
          printf("%s%15s:%4d " msg"%s\n",color,__FILE__,__LINE__,##arg,NONECOLOR); \
  }

//DBG("index:%d str = %s\n", index, str);
//輸出: test.c:main(146): index:2147483647 str = bejo
//DBG("index:%d str = %s\n", index, str);
//被置換成
//printf("%s%s(%d): " "index:%d str = %s\n", __FILE__, __FUNCTION__, __LINE__,  index, str);
//DBG --> printf
//msg --> "index:%d str = %s\n"
//arg --> index, str


typedef enum
{
	CMSRET_INITIALIZATION       = -1,    /**< Iintialization */
	CMSRET_SUCCESS              = 0,     /**< Success. */
	CMSRET_METHOD_NOT_SUPPORTED = 9000,  /**< Method not supported. */
	CMSRET_REQUEST_DENIED       = 9001,  /**< Request denied (no reason specified). */
	CMSRET_INTERNAL_ERROR       = 9002,  /**< Internal error. */
	CMSRET_INVALID_ARGUMENTS    = 9003,  /**< Invalid arguments. */
} CmsRet;
