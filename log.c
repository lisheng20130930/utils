#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "libos.h"
#include "log.h"


char* TimeToStrDateTime(char *strz)
{
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    
    sprintf(strz,"%04d-%02d-%02d %02d:%02d:%02d",ptm->tm_year+1900, ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    
    return strz;
}

static void _dump(const char *strz)
{
    FILE* hFile = NULL;
    char timeSTR[32] = {0};
    
    hFile = fopen(LOGFILENAME, "ab+");
    if (NULL != hFile) {
        fseek(hFile, 0, SEEK_END);
        fprintf(hFile,"%s %s" ,TimeToStrDateTime(timeSTR), strz);
        fclose(hFile);
    }
}

void _log(char* pszFormat, ...)
{        
    static char nPrintableStr[3*1024] = {0};
    va_list MyList;
    
    memset(nPrintableStr, 0x00, 3*1024);  
    
    va_start(MyList, pszFormat);
    _vsnprintf(nPrintableStr, 3*1024-1, pszFormat, MyList);
    va_end(MyList);
	
	// printf
	printf(nPrintableStr);

    /* write to file */
    _dump(nPrintableStr);
}
