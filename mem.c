#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "config.h"
#include "log.h"


#define PREFIX_SIZE (sizeof(size_t))


#define update_zmalloc_stat_alloc(__n) do{usedMemory += (__n);}while(0)
#define update_zmalloc_stat_free(__n)  do{usedMemory -= (__n);}while(0)


void *zmalloc(size_t size) {
    void *ptr = malloc(size+PREFIX_SIZE);
    if (!ptr){
		DBGPRINT(EERROR,("memory oo Exception, size = %u, used_size=%du\r\n",size,usedMemory));
		exit(0);
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
	usedMemory = 0;
}
