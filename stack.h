//////////////////////////////////////////////////////////////////////////
///  Stack Impl
/* 
The top index is defined -100, we i meet a Index as the top(-100), 
I will send the topest Item
*/
#ifndef CMMN_STACK_H
#define CMMN_STACK_H

#define MR_STACK_TOP_INDEX          (-100)

INT32
MR_StackCreate(TN_HANDLE *phStack);

BOOL
MR_StackIsEmpty(void *hStack);

INT32
MR_StackGetAt(void *hStack, INT32 iIndex, void **ppvItem);

INT32
MR_StackPush(void *hStack, void *pvItem, INT32 *piIndex);

void*
MR_StackPopup(void *hStack);

INT32
MR_StackDelete(void *hStack);

INT32
MR_StackClean(void *hStack);

INT32
MR_StackGetSize(void *hStack);


#endif