#include "libos.h"
#include "stdarg.h"
#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#endif

char* cmmn_strndup(const char *s, int len)
{
    char *s_new;
    
    if (s != NULL) {
        s_new = (char*)malloc(len + 1);
        if (s_new != NULL) {
            memcpy(s_new, s, len);
            s_new[len] = '\0';
        }
    }
    else {
        s_new = (char*)malloc(1);
        if (s_new != NULL) {
            s_new[0] = '\0';
        }
    }
    
    return s_new;
}

char* cmmn_strdup(const char *s)
{
	return cmmn_strndup(s,strlen(s));
}

int cmmn_strincmp(char* pszStr1, char* pszStr2,int iLen)
{
	int iRetval = 0;
    int i=0;
    do {
        if ((NULL == pszStr1 && pszStr2)||(pszStr1 && NULL == pszStr2)) {
            iRetval=-1;
            break;
        }
        if (NULL == pszStr1 && NULL == pszStr2) {
            break;
        }
        if ((!strlen(pszStr1)&&strlen(pszStr2))||(strlen(pszStr1)&&!strlen(pszStr2) )) {
            iRetval=-1;
            break;
        }
        if (!strlen(pszStr1)&&!strlen(pszStr2)) {
            break;
        }
        while(*pszStr1)
        {
            if (i == iLen) {
                break;
            }
            if (0 == *pszStr2) {
                iRetval =1;
                break;
            }
            if (__upchar(*pszStr1) - __upchar(*pszStr2) >0) {
                iRetval =1;
                break;
            }
            else if (__upchar(*pszStr1) - __upchar(*pszStr2) <0) {
                iRetval = -1;
                break;
            }
            pszStr1++;
            pszStr2++;
            i++;
        }
        
    } while(0);
    if (i!=iLen) {
        iRetval = -1;
    }
    return iRetval;
}

char* fmt2(const char * format, ...)
{
	static char number[1024] = {0};
	va_list MyList;
	memset(number, 0x00, 1024);
	va_start(MyList, format);
    _vsnprintf(number, 1023, format, MyList);
    va_end(MyList);
	return number;
}

int natoi(char *str, int n)
{
	char buffer[32] = {0};
	memcpy(buffer,str,__min(n,31));
	return atoi(buffer);
}

int nrand()
{
	srand((unsigned int)time(NULL)+rand());
	return rand();
}

#ifdef WIN32
static void cp_gettimeofday(struct timeval *tp, struct timezone *tz)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
}
#else
static void cp_gettimeofday(struct timeval *tp, struct timezone *tz)
{
    gettimeofday(tp,tz);
}
#endif

unsigned int geticktime()
{
    struct timeval tv;	
    cp_gettimeofday(&tv, NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}