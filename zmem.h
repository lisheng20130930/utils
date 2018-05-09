#ifndef __ZMEM_H
#define __ZMEM_H


unsigned int zusedsize();
void zclear();
void *zmalloc(size_t size);
void zfree(void *ptr);


#endif
