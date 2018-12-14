#ifndef CMMN_STACK_H
#define CMMN_STACK_H


#define MR_STACK_TOP_INDEX          (-100)


int MR_StackCreate(void **phStack);
bool MR_StackIsEmpty(void *hStack);
void* MR_StackGetAt(void *hStack, int iIndex);
int MR_StackPush(void *hStack, void *pvItem, int *piIndex);
void* MR_StackPopup(void *hStack);
int MR_StackDelete(void *hStack);
int MR_StackClean(void *hStack);
int MR_StackSize(void *hStack);


#endif
