#include "stdbool.h"
#include "mem.h"


#define M_MEM_BLOCK_PTRSIZE  (1024000)


#define M_MEM_INIT_SIZE ((M_MEM_NUM_FREE_LISTS + 2) * M_MEM_MIN_CHUNK_SIZE)
static void* DnMem_BlockInit(unsigned int iSize);
static void* DnMem_AllocateMemory(St_MemBlockHandle* pstMem, unsigned int uiRemSize, unsigned int *puiLen);


const static unsigned int s_Mem_auiMaxSizes[M_MEM_NUM_FREE_LISTS]={
    16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 0x8FFFFFFF  
};


static unsigned short Mem_ListIdx(unsigned int uiChunkSize)
{
    unsigned short l = 0;
	for(l = 0; s_Mem_auiMaxSizes[l] < (uiChunkSize + 1); l++);
	return l;
}

void* DnMem_HandleInit()
{
    St_MemHandle  *pstMemHandle = NULL;
    pstMemHandle = (St_MemHandle*)malloc(sizeof(St_MemHandle));

    if(NULL == pstMemHandle){        
        return NULL;
    }
    memset(pstMemHandle, 0X00, sizeof(St_MemHandle)); 

    pstMemHandle->MemInternal[0] = (St_MemBlockHandle*)DnMem_BlockInit(M_MEM_BLOCK_PTRSIZE);
    if (NULL == pstMemHandle->MemInternal[0]){
        free(pstMemHandle);
        return NULL;
    }
    pstMemHandle->uiAllocatedBytes = pstMemHandle->MemInternal[0]->uiAllocatedBytes;
    pstMemHandle->uiAllocateAll += M_MEM_BLOCK_PTRSIZE;
    return pstMemHandle;
}

static void* DnMem_BlockInit(unsigned int iSize)
{
    St_MemBlockHandle *pstMem = NULL;
    unsigned int uiSize = 0;
    int       i = 0;
    St_Chunk *pstTemp = NULL;

    if((iSize < M_MEM_MIN_BLOCK_SIZE) || (iSize > 0x7FFFFFFF)){
        return NULL;
    }
    pstMem = (St_MemBlockHandle *)malloc(sizeof(St_MemBlockHandle));
    if (NULL == pstMem){
        return NULL;
    }
    memset(pstMem, 0X00, sizeof(St_MemBlockHandle));
    pstMem->pucBasePtr = (unsigned char*)malloc(iSize);

    if(NULL == pstMem->pucBasePtr){        
        free(pstMem);
        return NULL;
    }
    
    memset(pstMem->pucBasePtr, 0x00, iSize);
    iSize &= ~M_MEM_MALLOC_ALIGN_MASK;
    /*make free chunk manage list*/
    for(i = 0; i < M_MEM_NUM_FREE_LISTS; i++){
        pstTemp = (St_Chunk *)(pstMem->pucBasePtr + i * M_MEM_MIN_CHUNK_SIZE);
        SET_HD1(pstTemp, 0);
        SET_HD2(pstTemp, (M_MEM_MIN_CHUNK_SIZE << 1) | 0x01);
        pstTemp->pstFwd = pstTemp;
        pstTemp->pstBck = pstTemp;
        pstMem->apstFreeLists[i] = pstTemp;
    }
    /*set first and last chunk */
    pstMem->pstFirstChunk = (St_Chunk *)(pstMem->pucBasePtr + M_MEM_NUM_FREE_LISTS * M_MEM_MIN_CHUNK_SIZE);
    SET_HD1(pstMem->pstFirstChunk, M_MEM_MIN_CHUNK_SIZE);
    uiSize = iSize - ((M_MEM_NUM_FREE_LISTS + 1) * M_MEM_MIN_CHUNK_SIZE);
    SET_HD2(pstMem->pstFirstChunk, uiSize << 1);
    ADD_TO_LIST(pstMem->apstFreeLists[Mem_ListIdx(uiSize)], pstMem->pstFirstChunk);
    pstMem->pstLastChunk = (St_Chunk *)(((unsigned char *)(pstMem->pucBasePtr)) + iSize - M_MEM_MIN_CHUNK_SIZE);
    SET_HD1(pstMem->pstLastChunk, uiSize);
    SET_HD2(pstMem->pstLastChunk, (M_MEM_MIN_CHUNK_SIZE << 1) | 0x01);
    
    pstMem->uiAllocatedBytes = (M_MEM_NUM_FREE_LISTS + 1) * M_MEM_MIN_CHUNK_SIZE;
    pstMem->iSize = iSize;
    return (void*)pstMem;
}

void* DnMem_Alloc(void* hMem, unsigned int uiSize)
{
    St_MemHandle *pstMemHandle = (St_MemHandle*)hMem;
    St_MemBlockHandle *pstMem = NULL;
    void* pstFound = NULL;
    unsigned int j = 0;
    unsigned int uiInit = 0;
    unsigned int uiNewSize = 0;
    unsigned int uiLen = 0;

    uiNewSize = REQUEST2SIZE(uiSize);
    for(j = 0; j <= pstMemHandle->uiMaxNo; j++){
        pstMem = pstMemHandle->MemInternal[j];
        pstFound = DnMem_AllocateMemory(pstMem, uiNewSize, &uiLen);
        if(NULL != pstFound){
            pstMemHandle->uiAllocatedBytes += uiLen;
            return pstFound;
        }
    }
	
    if((pstMemHandle->uiMaxNo >= M_MEM_MAX_BLOCK - 1)){
        return NULL;
    }

    if((uiNewSize + M_MEM_INIT_SIZE) > M_MEM_BLOCK_PTRSIZE){
		uiInit = uiNewSize + M_MEM_INIT_SIZE;
	}else{
		uiInit = M_MEM_BLOCK_PTRSIZE;
	}
	
    pstMemHandle->MemInternal[pstMemHandle->uiMaxNo + 1] = (St_MemBlockHandle*)DnMem_BlockInit(uiInit);
    if (NULL == pstMemHandle->MemInternal[pstMemHandle->uiMaxNo + 1]){
        return NULL;
    }
    pstMemHandle->uiAllocateAll += uiInit;
    pstMemHandle->uiMaxNo++;
    pstMem = pstMemHandle->MemInternal[pstMemHandle->uiMaxNo];
    pstMemHandle->uiAllocatedBytes += pstMem->uiAllocatedBytes;
    pstFound = DnMem_AllocateMemory(pstMem, uiNewSize, &uiLen);
    if (NULL != pstFound){ 
        pstMemHandle->uiAllocatedBytes += uiLen;
        return pstFound;
    }
    return NULL;
}

static void* DnMem_AllocateMemory(St_MemBlockHandle* pstMem, unsigned int uiNewSize, unsigned int *puiLen) 
{
    St_Chunk *pstTemp = NULL;
    St_Chunk *pstFound = NULL;
    St_Chunk *pstFreeList = NULL;
    St_Chunk *q = NULL;
    St_Chunk *pstNext = NULL; 
    St_Chunk *l = NULL;
    unsigned int uiRemSize = 0;
    unsigned int uiTempSize = 0;
    unsigned int uiSz = 0xFFFFFFFF;
    unsigned short i = 0;
    bool bFound = false;

    /*add the header*/
    for(i = Mem_ListIdx(uiNewSize); i < M_MEM_NUM_FREE_LISTS; i++){
        pstFreeList = pstMem->apstFreeLists[i];
        for(pstTemp = pstFreeList->pstFwd; (pstTemp != pstFreeList); pstTemp = pstTemp->pstFwd){
            uiTempSize= CHUNKSIZE(pstTemp);
            if(uiTempSize == uiNewSize){
                pstFound = pstTemp;
                uiSz = uiTempSize;
                bFound = true;
                break;
            }else if(uiTempSize > uiNewSize){
                /*make searching size is the most near*/
                if(uiTempSize < uiSz){
                    pstFound = pstTemp;
                    uiSz = uiTempSize;
                }
            }
        }
        if(pstFound != NULL){
            bFound = true;
            break;
        }
    }
    if(!bFound){
        return NULL;
    }
    
    /*remove chunk from free list*/
    REMOVE_FROM_LIST(pstFound);
    uiRemSize = uiSz - uiNewSize;
    /*if remain size is limited then alloc all apply size*/
    if(uiRemSize > M_MEM_MIN_CHUNK_SIZE){
        uiSz = uiNewSize;
        /*make the remain chunk*/
        q = (St_Chunk *)((unsigned char*)pstFound + uiSz);
        SET_HD2(q,uiRemSize << 1);
        SET_HD1(q,uiNewSize);
        /*modified next chunk's uiPrevSize*/		
        pstNext = NEXTCHUNK(q);
        pstNext->uiPrevSize = uiRemSize;
        l = pstMem->apstFreeLists[Mem_ListIdx(uiRemSize)];
        ADD_TO_LIST(l, q);
    }
    SET_HD2(pstFound, (uiSz << 1) | 0x01);
       
    pstMem->uiAllocatedBytes += CHUNKSIZE(pstFound);
    *puiLen = CHUNKSIZE(pstFound);

    return CHUNK2MEM(pstFound);
}

int DnMem_Free(void* hMem, void* hFree)
{
    St_MemHandle *pstMemHandle = (St_MemHandle*)hMem;
    St_MemBlockHandle *pstMem = NULL;
    St_Chunk *pstFirst = NULL;
    St_Chunk *pstLast = NULL;
	St_Chunk *pstTemp = NULL;
    St_Chunk *pstNext = NULL;
	St_Chunk *pstPrev = NULL;
	unsigned int uiFreeChunkSize = 0;
	unsigned int uiTempSize = 0;
	St_Chunk *l = NULL;
    unsigned int i = 0;
    int bFlag = false;

	if(NULL == hFree){
		return true;
	}
	
	pstTemp = MEM2CHUNK(hFree);
    for(i = 0; i <= pstMemHandle->uiMaxNo; i++){
        pstMem = pstMemHandle->MemInternal[i];
        if (NULL == pstMem){
            return false;
        }
        pstFirst = pstMem->pstFirstChunk;
        pstLast = pstMem->pstLastChunk;
        if ((pstTemp >= pstFirst) && (pstTemp < pstLast)){
            bFlag = true;
            break;
        }
    }

    if (!bFlag){
        return true;
    }

	if(CHUNK_ISFREE(pstTemp)){
	    return true;
	}
    uiFreeChunkSize = CHUNKSIZE(pstTemp);
	uiTempSize = uiFreeChunkSize;
	pstPrev = PREVCHUNK(pstTemp);
	pstNext = NEXTCHUNK(pstTemp);
    /*if prev chunk is freed, then united this chunk and prev chunk*/
	if(CHUNK_ISFREE(pstPrev)){
	    uiTempSize += CHUNKSIZE(pstPrev);
        pstTemp = pstPrev;
		REMOVE_FROM_LIST(pstPrev);
	}
	/*if next chunk is freed, then united this chunk and next chunk*/
	if(CHUNK_ISFREE(pstNext)){
	    uiTempSize += CHUNKSIZE(pstNext);
        REMOVE_FROM_LIST(pstNext);
	}
	SET_HD2(pstTemp,uiTempSize << 1);
	pstNext = NEXTCHUNK(pstTemp);
	pstNext->uiPrevSize = uiTempSize;
	l = pstMem->apstFreeLists[Mem_ListIdx(uiTempSize)];
	ADD_TO_LIST(l,pstTemp);
	pstMem->uiAllocatedBytes -= uiFreeChunkSize;
    pstMemHandle->uiAllocatedBytes -= uiFreeChunkSize;
	
	return true;
}

void DnMem_HandleUint(void* hMem)
{
    St_MemHandle *pstMemHandle = (St_MemHandle *)hMem;
    St_MemBlockHandle *pstMem = NULL;
    unsigned int i = 0;

    for(i = 0; i <= pstMemHandle->uiMaxNo; i++){
        pstMem = pstMemHandle->MemInternal[i];
        if(NULL == pstMem){
            continue;
        }
        if(pstMem->pucBasePtr){
            free(pstMem->pucBasePtr);
            pstMem->pucBasePtr = NULL;
        }
        free(pstMem);
    }
    free(pstMemHandle);
}

unsigned int DnMem_UsedMemorySize(void* hMem)
{
	St_MemHandle *pstMemHandle = (St_MemHandle *)hMem;
	if(pstMemHandle){
		return pstMemHandle->uiAllocateAll;
	}
	return 0;
}
