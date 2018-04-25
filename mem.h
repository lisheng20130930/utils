#ifndef MEM_INTERANL_H
#define MEM_INTERANL_H


#include "stdlib.h"
#include "string.h"


#define M_MEM_CHUNK_HEADER_SIZE   (2 * sizeof(unsigned int))
#define M_MEM_MIN_CHUNK_SIZE sizeof(St_Chunk)
#define M_MEM_MALLOC_ALIGN_MASK 3


#define M_MEM_MAX_BLOCK  (32)
#define M_MEM_NUM_FREE_LISTS 10
#define M_MEM_MIN_BLOCK_SIZE (M_MEM_NUM_FREE_LISTS+2) * M_MEM_MIN_CHUNK_SIZE

#define CHUNK2MEM(p)  ((void*)(( unsigned char *)(p) + M_MEM_CHUNK_HEADER_SIZE))
#define MEM2CHUNK(q)  ((P_St_Chunk)((unsigned char *)(q) - M_MEM_CHUNK_HEADER_SIZE))


#define CHUNKSIZE(p)        (((p)->uiSize & ~0x01) >> 1)
#define SET_HD1(p, v)   ((p)->uiPrevSize = (unsigned int)(v))
#define SET_HD2(p, v)   ((p)->uiSize = (unsigned int)(v))

#define PREVCHUNK(p)    ((St_Chunk *)(((unsigned char *)(p)) - p->uiPrevSize))
#define NEXTCHUNK(p)    ((St_Chunk *)(((unsigned char *)(p)) + CHUNKSIZE (p)))

#define CHUNK_ISFREE(p) ((((St_Chunk *)(p))->uiSize & 0X01) == 0)

#define REMOVE_FROM_LIST(p) (p)->pstFwd->pstBck = (p)->pstBck; (p)->pstBck->pstFwd = (p)->pstFwd;
#define ADD_TO_LIST(l, p) (p)->pstFwd = (l)->pstFwd; (p)->pstBck = l; (l)->pstFwd->pstBck = p; (l)->pstFwd = p;

#define REQUEST2SIZE(req) (((unsigned int)((req) + M_MEM_CHUNK_HEADER_SIZE + M_MEM_MALLOC_ALIGN_MASK) < (unsigned int)( M_MEM_MIN_CHUNK_SIZE + M_MEM_MALLOC_ALIGN_MASK)) ? M_MEM_MIN_CHUNK_SIZE : (unsigned int)(((req) + M_MEM_CHUNK_HEADER_SIZE + M_MEM_MALLOC_ALIGN_MASK) & ~( M_MEM_MALLOC_ALIGN_MASK)))


typedef struct tagSt_Chunk{	
    unsigned int        uiPrevSize; 
   	unsigned int        uiSize;
	struct tagSt_Chunk  *pstFwd;
    struct tagSt_Chunk  *pstBck;
}St_Chunk, *P_St_Chunk;


typedef struct tagSt_MemBlockHandle{
    unsigned char  *pucBasePtr;
    St_Chunk  *pstFirstChunk;
	St_Chunk  *pstLastChunk;
	St_Chunk  *apstFreeLists[M_MEM_NUM_FREE_LISTS];
	unsigned int  uiAllocatedBytes;
	unsigned int iSize;
}St_MemBlockHandle, *P_St_MemBlockHandle;


typedef struct tagSt_MemHandle{
    St_MemBlockHandle    *MemInternal[M_MEM_MAX_BLOCK];
    unsigned int         uiMaxNo;
    unsigned int  uiAllocatedBytes;
    unsigned int   uiAllocateAll;
}St_MemHandle, *P_St_MemHandle;



void* DnMem_Alloc(void* hMem, unsigned int uiSize);
int  DnMem_Free(void* hMem,void* hFree);


unsigned int DnMem_UsedMemorySize(void* hMem);
void* DnMem_HandleInit();
void DnMem_HandleUint(void* hMem);



//////////////////////////////////////////////////////////////////////////
///////////////
#define CMMN_MALLOC(i)   DnMem_Alloc(gMem,i);
#define CMMN_FREE(p)     DnMem_Free(gMem,p);
#define MEM_USED_SIZE()  DnMem_UsedMemorySize(gMem)


extern void* gMem;


#endif
