#include "libos.h"
#include "zmem.h"


#define PREFIX_SIZE (sizeof(size_t))


#define update_zmalloc_stat_alloc(__n) do{g_usedMemory += (__n);}while(0)
#define update_zmalloc_stat_free(__n)  do{g_usedMemory -= (__n);}while(0)


static unsigned int g_usedMemory = 0;


void *zmalloc(size_t size) {
    void *ptr = malloc(size+PREFIX_SIZE);
    if (!ptr){		
		return NULL;
    }
    *((size_t*)ptr) = size;
    update_zmalloc_stat_alloc(size+PREFIX_SIZE);
    return (char*)ptr+PREFIX_SIZE;
}

void zfree(void *ptr) {
	void *realptr = NULL;
    size_t oldsize;
    if (ptr == NULL) return;
    realptr = (char*)ptr-PREFIX_SIZE;
    oldsize = *((size_t*)realptr);
    update_zmalloc_stat_free(oldsize+PREFIX_SIZE);
    free(realptr);
}

void zclear()
{
	g_usedMemory = 0;
}

unsigned int zusedsize()
{
	return g_usedMemory;
}
