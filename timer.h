#ifndef TIMER_H
#define TIMER_H


/* 采用数组的格式, 数组链表! 容易索引和删除 */
#define NTIMER		(24000)


typedef bool (*pfn_onTimerMessage)(void *pUsr, unsigned int dwTimerID);


/* Time event structure */
typedef struct aeTimeEvent{
	struct aeTimeEvent *next; /* NEXT */
    unsigned int id; /* time event identifier. */
    long when_sec; /* seconds */
    long when_ms; /* milliseconds */
    pfn_onTimerMessage timeProc;
    void *pUsr;
}aeTimeEvent;


// 不同作用的dwTimerID的值一定是不要重复的
//  pUsr+dwTimerID作为关键字

bool SetGameTimer(void *pUsr, unsigned int dwTimerID, pfn_onTimerMessage pfn, unsigned long dwElapse, int dwRepeat, unsigned int wBind);
bool KillGameTimer(void *pUsr, unsigned int dwTimerID);


int processTimeEvents();


#endif