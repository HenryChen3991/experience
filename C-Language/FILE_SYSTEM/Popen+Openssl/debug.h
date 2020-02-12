#define DEBUG_FLAG 1
#define DEBUG(msg,arg...) printf("%s:%s:%d " msg"\n",__FILE__,__FUNCTION__,__LINE__,##arg)

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
