#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "libos.h"
#include "log.h"


#if(defined(__IOS__)||defined(__ANDROID__))
static __inline char* log_filename(char *name)
{
    static char g_name[1024] = {0};
    extern char *g_dataPathDir; //android,ios has its datadir
    sprintf(g_name,"%s/%s",g_dataPathDir,name);
    return g_name;
}
#endif

#define TRACE_HEADER_LEN (35)

char* traceHeader(char *strz, int len)
{
	static unsigned int g_lifeCycle = 0;	
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);

	if(g_lifeCycle==0){
		g_lifeCycle = geticktime();
	}
    
    sprintf(strz,"%04d%u@%04d-%02d-%02d %02d:%02d:%02d",len,g_lifeCycle,ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    strz[TRACE_HEADER_LEN-1] = ' ';
    return strz;
}

static void _dump(char *buffer,int size)
{
    FILE* hFile = NULL;
    hFile = fopen(LOGFILENAME, "ab+");
    if (NULL != hFile) {
        fwrite(buffer,1,size,hFile);
        fclose(hFile);
    }
}

void _log(char* pszFormat, ...)
{        
    static char nPrintableStr[4096] = {0};
    va_list MyList;
	        
    va_start(MyList, pszFormat);
    _vsnprintf(nPrintableStr+TRACE_HEADER_LEN, 4095-TRACE_HEADER_LEN, pszFormat, MyList);
    va_end(MyList);
	
	int len = strlen(nPrintableStr+TRACE_HEADER_LEN);
	traceHeader((char*)nPrintableStr,len);
	
	// printf
	#ifndef __ANDROID__
	printf(nPrintableStr);
	#endif

    /* write to file */
    _dump(nPrintableStr,TRACE_HEADER_LEN+len);
}
