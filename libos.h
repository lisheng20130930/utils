#ifndef LIBOS_H
#define LIBOS_H


#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include "math.h"
#include "log.h"
#include "stdbool.h"
#ifdef WIN32
#include <io.h>
#include <direct.h> 
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif


#ifndef NULL
#define NULL  ((void*)0)
#endif


#ifndef INT_MAX
#define INT_MAX  (2147483647)
#endif

#define __upchar(a)    (((a)>='a' && (a)<='z')?((a)-0x20):(a))

#ifndef __min
#define __min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef __max
#define __max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef __abs
#define __abs(x) (((x)<0) ? -(x) : (x))
#endif


#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif


#ifndef WIN32
#define stricmp strcasecmp
#define _vsnprintf vsnprintf
#define _snprintf snprintf
#define strnicmp strncasecmp
#else
#define SIGHUP   (1)
#endif



char* cmmn_strndup(const char *s, int len);
char* cmmn_strdup(const char *s);
int   cmmn_strincmp(char* pszStr1, char* pszStr2,int iLen);
char* fmt2(const char * format, ...);
int   natoi(char *str, int n);
int   nrand();
unsigned int geticktime();



#endif