#include "libos.h"
#include "stack.h"


#define MR_STACK_EXPOND_STEP        (16)


typedef struct tagSt_MrStack{
	void  **pObjArray;
	int   iArraySize;
	int   iCurrSize;
}St_MrStack;


int MR_StackCreate(void* *phStack)
{
	St_MrStack *pMe = (St_MrStack*)malloc(sizeof(St_MrStack));
	memset(pMe, 0x00, sizeof(St_MrStack));
	*phStack = (void*)pMe;
	return 0;
}

void* MR_StackGetAt(void* hStack, int index)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	if(MR_StackIsEmpty(hStack)){
		return NULL;
	}

	if(index >= pMe->iCurrSize){
		return NULL;
	}

	if(MR_STACK_TOP_INDEX == index){
		index = pMe->iCurrSize - 1;
	}

	return pMe->pObjArray[index];
}

bool MR_StackIsEmpty(void* hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	return (0 == pMe->iCurrSize);
}

static int _MR_Expond(St_MrStack *pMe)
{
	void **pvBark = pMe->pObjArray;	
	pMe->pObjArray= (void**)malloc((pMe->iArraySize + MR_STACK_EXPOND_STEP) * sizeof(void*));
	memset(pMe->pObjArray, 0x00, (pMe->iArraySize + MR_STACK_EXPOND_STEP) * sizeof(void*));
	memcpy(pMe->pObjArray, pvBark, pMe->iArraySize * sizeof(void*));
	pMe->iArraySize += MR_STACK_EXPOND_STEP;
	free(pvBark);
	return 0;
}

int MR_StackPush(void *hStack, void *pvItem, int *piIndex)
{
	St_MrStack *pMe = (St_MrStack*)hStack;
	int iRet = 0;

	if(pMe->iCurrSize == pMe->iArraySize){
		iRet = _MR_Expond(pMe);
	}

	if(iRet != 0){
		return iRet;
	}
	
	*piIndex = pMe->iCurrSize;
	pMe->pObjArray[pMe->iCurrSize ++] = pvItem;

	return 0;
}

void* MR_StackPopup(void* hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	if(MR_StackIsEmpty(hStack)){
		return NULL;
	}

	return pMe->pObjArray[pMe->iCurrSize-- - 1];
}

int MR_StackClean(void* hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;
	
	free(pMe->pObjArray);

	pMe->iArraySize = 0;
	pMe->iCurrSize = 0;
	pMe->pObjArray = NULL;

	return 0;
}

int MR_StackDelete(void* hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	MR_StackClean(hStack);

	free(pMe);

	return 0;
}

int MR_StackSize(void* hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	return pMe->iCurrSize;
}
