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

char* traceHeader(char *strz, int len)
{
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    
    sprintf(strz,"%04d-%02d-%02d %02d:%02d:%02d <%d>",ptm->tm_year+1900, ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec, len);
    
    return strz;
}

static void _dump(int len, char *strz)
{
    FILE* hFile = NULL;
    char szHeader[64] = {0};
    
    hFile = fopen(LOGFILENAME, "ab+");
    if (NULL != hFile) {
        fseek(hFile, 0, SEEK_END);
        fprintf(hFile,"%s %s" ,traceHeader(szHeader,len), strz);
        fclose(hFile);
    }
}

void _log(char* pszFormat, ...)
{        
    static char nPrintableStr[4096] = {0};
    va_list MyList;
	int len = 0;
    
    memset(nPrintableStr, 0x00, 4096);  
    
    va_start(MyList, pszFormat);
    len =_vsnprintf(nPrintableStr, 4096-1, pszFormat, MyList);
    va_end(MyList);
	
	// printf
	#ifndef __ANDROID__
	printf(nPrintableStr);
	#endif

    /* write to file */
    _dump(__min(len,4096),nPrintableStr);
}
