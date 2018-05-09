#ifndef __ZMALLOC_H
#define __ZMALLOC_H


void *zmalloc(size_t size);
void zfree(void *ptr);
void zclear();


#endif
