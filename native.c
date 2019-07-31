#include "libos.h"
#include "native.h"
#include "Idmap.h"
#ifdef WIN32
#include "winsock2.h"
#else
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#endif


#define MAXHANDLER (256)

struct NSHandler{
	void (*cb)(int code, char *result);
	char *cbName;
};


static Idmap *ghMap = NULL;


static native_callback popHandler(const char *cbName)
{
	native_callback cb = NULL;
	if(NULL==ghMap){
		return NULL;
	}
	
	struct NSHandler *pHandle = (struct NSHandler*)Idmap_get(ghMap,cbName);
	if(NULL==pHandle){
		return NULL;
	}
	
	cb = pHandle->cb;

	Idmap_remove(ghMap,cbName);
	free(pHandle);

	return cb;
}


static bool pushHandler(char* cbName, native_callback cb)
{
	if(!ghMap){
		ghMap = Idmap_create(MAXHANDLER);
	}
	
	struct NSHandler *pHandle = (struct NSHandler*)malloc(sizeof(struct NSHandler));
	memset(pHandle,0x00,sizeof(struct NSHandler));
	
	pHandle->cbName = cmmn_strdup(cbName);
	pHandle->cb = cb;
	
	Idmap_put(ghMap,pHandle->cbName,pHandle);
	
	return true;
}

void nb_routerCallback(char *cbName, int code, char *result)
{
	native_callback cb = popHandler(cbName);
	if(cb){
		cb(code, result);
	}
}

static char* getRandomKey()
{
	static char gKey[32] = {0};
	struct timeval tv = {0};	
    cp_gettimeofday(&tv, NULL);
	sprintf(gKey,"%u%u",tv.tv_sec,tv.tv_usec);
	return gKey;
}

void nb_callRouter(char *method, char *reqStr, native_callback cb)
{
	extern void plat_callRouter(char*, char*, char*);
	char *cbName = getRandomKey();
	pushHandler(cbName,cb);
	
	plat_callRouter(cbName,method,reqStr);
}