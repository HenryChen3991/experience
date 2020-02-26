// FILE: DebugLog.h
// REMARKS: This is a generic pair of files useful for debugging.  It provides three levels of 
// debug logging, currently; in addition to disabling it.  Level 3 is the most information.
// Levels 2 and 1 have progressively more.  Thus, you can write: 
//     DEBUGLOG_LOG(1, "a number=%d", 7);
// and it will be seen if DEBUG is anything other than undefined or zero.  If you write
//     DEBUGLOG_LOG(3, "another number=%d", 15);
// it will only be seen if DEBUG is 3.  When not being displayed, these routines compile
// to NOTHING.  I reject the argument that debug code needs to always be compiled so as to 
// keep it current.  I would rather have a leaner and faster app, and just not be lazy, and 
// maintain debugs as needed.  I don't know if this works with the C preprocessor or not, 
// but the rest of the code is fully C compliant also if it is.

//#define DEBUG 2

#ifdef DEBUG
#define DEBUGLOG_INIT(filename) debuglog_init(filename)
#else
#define debuglog_init(...)
#endif

#ifdef DEBUG
int a;
#define DEBUGLOG_CLOSE debuglog_close
#else
#define debuglog_close(...)
#endif

#define DEBUGLOG_LOG(level, fmt, ...) DEBUGLOG_LOG ## level (fmt, ##__VA_ARGS__)

#if DEBUG == 0
#define DEBUGLOG_LOG0(...)
#endif

#if DEBUG >= 1
#define DEBUGLOG_LOG1(fmt, ...) debuglog_log (fmt, ##__VA_ARGS__)
#else
#define DEBUGLOG_LOG1(...)
#endif

#if DEBUG >= 2
#define DEBUGLOG_LOG2(fmt, ...) debuglog_log (fmt, ##__VA_ARGS__)
#else
#define DEBUGLOG_LOG2(...)
#endif

#if DEBUG == 3
#define DEBUGLOG_LOG3(fmt, ...) debuglog_log (fmt, ##__VA_ARGS__)
#else
#define DEBUGLOG_LOG3(...)
#endif

void debuglog_init(char *filename);
void debuglog_close(void);
void debuglog_log(char* format, ...);
