#ifndef TIMER_H
#define TIMER_H


/* ��������ĸ�ʽ, ��������! ����������ɾ�� */
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


// ��ͬ���õ�dwTimerID��ֵһ���ǲ�Ҫ�ظ���
//  pUsr+dwTimerID��Ϊ�ؼ���

bool SetGameTimer(void *pUsr, unsigned int dwTimerID, pfn_onTimerMessage pfn, unsigned long dwElapse, int dwRepeat, unsigned int wBind);
bool KillGameTimer(void *pUsr, unsigned int dwTimerID);


int processTimeEvents();


#endif