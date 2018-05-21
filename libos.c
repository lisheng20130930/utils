#include "libos.h"
#include "stdarg.h"


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