#ifndef  ENCPYT_H
#define ENCPYT_H

#include "stdbool.h"
#include "stdlib.h"

char* DES_Encrypt(char *sourceData, int sourceSize, char *keyStr, int *resultSize);
char* DES_Decrypt(char *sourceData, int sourceSize, char *keyStr, int* resultSize);


#endif
