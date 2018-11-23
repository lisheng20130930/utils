#include "libos.h"
#include "timer.h"

#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#endif

static time_t g_lastTime = 0L;     /* Used to detect system clock skew */
static int g_maxIdx = (-1);

static aeTimeEvent *g_timeEvent[NTIMER] = {0};


#ifdef WIN32
void cp_gettimeofday(struct timeval *tp, struct timezone *tz)
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
void cp_gettimeofday(struct timeval *tp, struct timezone *tz)
{
    gettimeofday(tp,tz);
}
#endif

static void aeGetTime(long *seconds, long *milliseconds)
{
    struct timeval tv;
	
    cp_gettimeofday(&tv, NULL);
    *seconds = tv.tv_sec;
    *milliseconds = tv.tv_usec/1000;
}

static void aeAddMillisecondsToNow(unsigned long milliseconds, long *sec, long *ms) {
    long cur_sec, cur_ms, when_sec, when_ms;
	
    aeGetTime(&cur_sec, &cur_ms);
    when_sec = cur_sec + milliseconds/1000;
    when_ms = cur_ms + milliseconds%1000;
    if (when_ms >= 1000) {
        when_sec ++;
        when_ms -= 1000;
    }
    *sec = when_sec;
    *ms = when_ms;
}


static void HungTimer(int j, void *pUsr, unsigned int dwTimerID, pfn_onTimerMessage pfn, unsigned long dwElapse)
{
	aeTimeEvent *te = (aeTimeEvent*)malloc(sizeof(aeTimeEvent));
	memset(te,0x00,sizeof(aeTimeEvent));
	
	te->id = dwTimerID;
	te->pUsr = pUsr;
	te->timeProc = pfn;	
	aeAddMillisecondsToNow(dwElapse,&te->when_sec,&te->when_ms);
    
	g_timeEvent[j] = te;
	
	// Update maxIdx
	if(g_maxIdx<j){
		g_maxIdx = j;
	}
}


bool SetGameTimer(void *pUsr, unsigned int dwTimerID, pfn_onTimerMessage pfn, unsigned long dwElapse, int dwRepeat, unsigned int wBind)
{
	//DBGPRINT(EERROR,("[TIMER] SetGameTimer pUsr=0x%x, dwTimerID=%d\r\n",pUsr,dwTimerID));
	int i;
	bool found = false;
	int j = 0;
	
	for(i=0;i<NTIMER;i++){
		if(!g_timeEvent[i]){			
			j = i;
			break;
		}
		if(pUsr==g_timeEvent[i]->pUsr&&dwTimerID==g_timeEvent[i]->id){
			found = true;
			break;
		}
	}
	
	if(found){
		return true;
	}

	if((-1)==j){
		return false;
	}

	HungTimer(j,pUsr,dwTimerID,pfn,dwElapse);
	return true;
}


static void unHungTimer(int i)
{
	free(g_timeEvent[i]);
	g_timeEvent[i]=NULL;
	
	// Update maxIdx
	if(g_maxIdx==i&&g_maxIdx>=0){
		int _max = (-1);
		for(i=g_maxIdx-1;i>=0;i--){
			if(g_timeEvent[i]){
				_max = i;
				break;
			}
		}
		g_maxIdx = _max;
	}
}


bool KillGameTimer(void *pUsr, unsigned int dwTimerID)
{
	int i;
	bool found = false;
	
	//DBGPRINT(EERROR,("[TIMER] KillTimer pUsr=0x%x,dwTimerID=%d\r\n",pUsr,dwTimerID));
	for(i=0;i<(g_maxIdx+1);i++){
		if(!g_timeEvent[i]){
			continue;
		}		
		if(pUsr==g_timeEvent[i]->pUsr&&dwTimerID==g_timeEvent[i]->id){
			found = true;
			break;
		}
	}
	
	if(found){
		unHungTimer(i);
		return true;
	}

	return true;
}

int processTimeEvents()
{
    int processed = 0;
    aeTimeEvent *te;    
    time_t now = time(NULL);
	int i;
	int _max = g_maxIdx;
	
    /* If the system clock is moved to the future */
    if (now < g_lastTime) {
        for(i=0;i<_max;i++) {
			if(!g_timeEvent[i]) {
				continue;
			}
			g_timeEvent[i]->when_sec = 0;
		}
    }
    g_lastTime = now;
	    
    for(i=0;i<(_max+1);i++){
        long now_sec, now_ms;
        
		te = g_timeEvent[i];
		if(!te){
			continue;
		}
		
        aeGetTime(&now_sec, &now_ms);
        if(now_sec > te->when_sec 
			|| (now_sec == te->when_sec && now_ms >= te->when_ms))
        {
			pfn_onTimerMessage timeProc = te->timeProc;
			void *pUsr = te->pUsr;
			unsigned int id = te->id;
			unHungTimer(i); // 只支持单次触发的TIMER，重复TIMER尚未用到, 暂不支持
            timeProc(pUsr, id);
        }
    }

    return processed;
}