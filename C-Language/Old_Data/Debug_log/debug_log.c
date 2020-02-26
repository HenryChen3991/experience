// FILE: DebugLog.h
// REMARKS: This is a generic pair of files useful for debugging.  It provides three levels of 
// debug logging, currently; in addition to disabling it.  See DebugLog.h's remarks for more 
// info.

#include <stdio.h>
#include <stdarg.h>

#include "debug_log.h"

FILE *hndl;
char *savedFilename;

void debuglog_init(char *filename)
{
    savedFilename = filename;
    hndl = fopen(savedFilename, "wt");
    fclose(hndl);
}

void debuglog_close(void)
{
    //fclose(hndl);
}

void debuglog_log(char* format, ...)
{
    hndl = fopen(savedFilename,"at");
    va_list argptr;
    va_start(argptr, format);
    vfprintf(hndl, format, argptr);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
//    fputc('\n',hndl);
    fclose(hndl);
}



/*
//example:
//VARARGS
static void
ndo_error(netdissect_options *ndo, const char *fmt, ...)
{
	va_list ap;

	if(ndo->program_name)
		(void)fprintf(stderr, "%s: ", ndo->program_name);
	va_start(ap, fmt);
	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);
	if (*fmt) {
		fmt += strlen(fmt);
		if (fmt[-1] != '\n')
			(void)fputc('\n', stderr);
	}
	exit(1);
}
*/
