#include "libos.h"
#include "native.h"
#include "Idmap.h"


#define MAXHANDLER (256)

struct NSHandler{
	native_callback cb;
	char *cbName;
};


static unsigned int g_seed = 0;
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

static char* getRandomKey(char *method)
{
	static char gKey[64] = {0};	
	sprintf(gKey,"%u@%s",g_seed,method);
	g_seed++;
	return gKey;
}

#ifdef WIN32
void plat_callRouter(char*, char*, char*){}
#endif

void nb_callRouter(char *method, char *reqStr, native_callback cb)
{
	extern void plat_callRouter(char*, char*, char*);
	char *cbName = getRandomKey(method);
	pushHandler(cbName,cb);	
	plat_callRouter(cbName,method,reqStr);
}