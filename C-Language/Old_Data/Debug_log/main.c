#include <stdio.h>
#include <stdlib.h>
#include "debug_log.h"

void main(void){
	int x = 5;
	DEBUGLOG_INIT("afile.log");
	DEBUGLOG_LOG(1, "loglevel 1 the value is: %d %.2f %s\n", x,7.1,"vvv");
	DEBUGLOG_LOG(3, "loglevel 3 the value is: %d %.2f %s\n", x,6.5,"aaa");
	DEBUGLOG_CLOSE();
}

