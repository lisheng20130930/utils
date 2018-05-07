#include "cp_3thd_interface.h"
#include "cmmn_cmmnlib.h"
#include "cmmn_stack.h"

#define MR_STACK_EXPOND_STEP        (16)


typedef struct tagSt_MrStack
{
	void  **pObjArray;
	INT32   iArraySize;
	INT32   iCurrSize;
}St_MrStack;

INT32
MR_StackCreate(TN_HANDLE *phStack)
{
	St_MrStack *pMe = NULL;

	pMe =(St_MrStack*)CMMN_MALLOC(sizeof(St_MrStack));
	if(NULL == pMe)
	{
		return TN_NOMEMORY;
	}

	CMMN_MEMSET(pMe, 0x00, sizeof(St_MrStack));

	*phStack = (TN_HANDLE)pMe;

	return TN_SUCCESS;
}

INT32
MR_StackGetAt(TN_HANDLE hStack, INT32 iIndex, void **ppvObj)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	if(MR_StackIsEmpty(hStack))
	{
		return TN_FAILED;
	}

	if(iIndex >= pMe->iCurrSize)
	{
		return TN_FAILED;
	}

	if(MR_STACK_TOP_INDEX == iIndex)
	{
		iIndex = pMe->iCurrSize - 1;
	}

	*ppvObj = pMe->pObjArray[iIndex];

	return TN_SUCCESS;
}

BOOL
MR_StackIsEmpty(TN_HANDLE hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	return (0 == pMe->iCurrSize);
}

static INT32
_MR_Expond(St_MrStack *pMe)
{
	void **pvBark = NULL;

	pvBark = pMe->pObjArray;
	
	pMe->pObjArray= (void**)CMMN_MALLOC((pMe->iArraySize + MR_STACK_EXPOND_STEP) * sizeof(void*));
	if(NULL == pMe->pObjArray)
	{
		pMe->pObjArray = pvBark;
		return TN_NOMEMORY;
	}

	CMMN_MEMSET(pMe->pObjArray, 0x00, (pMe->iArraySize + MR_STACK_EXPOND_STEP) * sizeof(void*));

	CMMN_MEMCPY(pMe->pObjArray, pvBark, pMe->iArraySize * sizeof(void*));
	
	pMe->iArraySize += MR_STACK_EXPOND_STEP;

	CMMN_FREE(pvBark);

	return TN_SUCCESS;
}


INT32
MR_StackPush(void *hStack, void *pvItem, INT32 *piIndex)
{
	St_MrStack *pMe = (St_MrStack*)hStack;
	INT32 iRet = TN_SUCCESS;

	if(pMe->iCurrSize == pMe->iArraySize)
	{
		iRet = _MR_Expond(pMe);
	}

	if(iRet != TN_SUCCESS)
	{
		return iRet;
	}
	
	*piIndex = pMe->iCurrSize;
	pMe->pObjArray[pMe->iCurrSize ++] = pvItem;

	return TN_SUCCESS;
}

void*
MR_StackPopup(TN_HANDLE hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	if(MR_StackIsEmpty(hStack))
	{
		return NULL;
	}

	return pMe->pObjArray[pMe->iCurrSize-- - 1];
}

INT32
MR_StackClean(TN_HANDLE hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;
	
	CMMN_FREE(pMe->pObjArray);

	pMe->iArraySize = 0;
	pMe->iCurrSize = 0;
	pMe->pObjArray = NULL;

	return TN_SUCCESS;
}

INT32
MR_StackDelete(TN_HANDLE hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	MR_StackClean(hStack);

	CMMN_FREE(pMe);

	return TN_SUCCESS;
}

INT32
MR_StackGetSize(TN_HANDLE hStack)
{
	St_MrStack *pMe = (St_MrStack*)hStack;

	return pMe->iCurrSize;
}