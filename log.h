#ifndef LOG_H
#define LOG_H


extern char* LOGFILENAME; /* logger */

/* logger level */
enum{
    EDEBUG = 0x01,
	EMSG   = 0x02,
	EERROR = 0x04,
	ELEVEL = EDEBUG|EMSG|EERROR
};

#define DBGPRINT(t,x)   {do{if(ELEVEL&(t)){_log x;}}while(0);}
#define DASSERT(x)  {do{if(!(x)){DBGPRINT(EERROR,("Assert...\r\n"));}}while(0);}

char* TimeToStrDateTime(char *strz);

void _log(char* pszFormat, ...);


#endif
