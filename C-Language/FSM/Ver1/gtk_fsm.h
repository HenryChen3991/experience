
#define DEBUG_FLAG 1

#if DEBUG_FLAG
#define DEBUG(msg,arg...) printf("%10s:%20s:%4d " msg"\n",__FILE__,__FUNCTION__,__LINE__,##arg)
#else
#define DEBUG(msg,arg...)
#endif//DEBUG_FLAG

#define ERROR(msg,arg...) printf("%10s:%20s:%4d " msg"\n",__FILE__,__FUNCTION__,__LINE__,##arg)



#define SUCCESS 1
#define FAILED  0

struct eocPacket{
    int tmp;
} ;

struct eocStack{
    int seq;
    struct eocPacket *rxBuffer;
    struct eocPacket *txBuffer;
};
