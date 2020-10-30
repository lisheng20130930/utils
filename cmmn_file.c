#include "libos.h"
#include "cmmn_file.h"
#include "windows.h"


int Dn_Gb2Ucs2(const char * src, char *dest, int srcLen)
{
    return MultiByteToWideChar(CP_ACP,0,src,-1,(LPWSTR)dest, srcLen >> 1);
}

static int DnMid_WideCharToMultiByte(char * src, char * dst, int iLen)
{
    memcpy(dst, src, strlen(src));
    #if 0
    int iNeed = WideCharToMultiByte(CP_ACP, 0, (const unsigned short *)src, -1, NULL, 0, NULL, NULL);
    if (iNeed >= iLen){
        return 0;
    }else{
        WideCharToMultiByte(CP_ACP, 0, (const unsigned short *)src, -1, dst, iNeed, NULL, NULL);
    }
    #endif
    dst[strlen(src)] = '\0';
    return 0;
}

int cmmn_findFirstFile(const char* name, char* buffer, int len)
{
	short awcDest[CMMN_FILE_MAX_PATH + 1] = {0};
	short awCurDir[MAX_PATH] = {0};
	short *pwName = NULL;
	WIN32_FIND_DATA stData = {0};
	int iNameLen = 0;
	HANDLE hFile = NULL;
	char acPath[CMMN_FILE_MAX_PATH + 1] = {0};
	char newName[CMMN_FILE_MAX_PATH + 1] = {0};
	int iRet = 0;
	if (NULL == name || NULL == buffer){
		return 0;
	}
    iNameLen = strlen(name);
	if (iNameLen > CMMN_FILE_MAX_PATH){
		return 0;
	}
	if (0 == iNameLen){
		DWORD dwLen = 0;
		dwLen = GetCurrentDirectory(MAX_PATH, (char*)awCurDir);
		if (dwLen == 0)
		{
			return 0;
		}
		pwName = awCurDir;
	}else{		
	    char c = name[iNameLen - 1];
		if (c =='/' || c == '\\'){
			memcpy(newName, name, iNameLen);
			newName[iNameLen] = '*';
			newName[iNameLen + 1] = '\0';
			name = newName;
		}
        sprintf(acPath, "%s",name);
        Dn_Gb2Ucs2(acPath, (char*)awcDest, (CMMN_FILE_MAX_PATH + 1) << 1);
		pwName = awcDest;
	}
	hFile = FindFirstFile((const char*)"*.*", &stData);
	if (INVALID_HANDLE_VALUE == hFile){
		return 0;
	}	
	if (0 != (iRet = DnMid_WideCharToMultiByte((char*)stData.cFileName, buffer, len))){
		return 0;
	}
	return (int)hFile;
}

int cmmn_findNextFile(int search_handle, char* buffer, int len)
{
	int bRet = FALSE;
	WIN32_FIND_DATA stData = {0};
	int iRet = 0;
	if (search_handle < 0){
		return 0;
	}
	if (NULL == buffer){
		return 0;
	}
	bRet = FindNextFile((HANDLE)search_handle, &stData);
	if (bRet == FALSE){
		if (GetLastError() == ERROR_NO_MORE_FILES){
			return 0;
		}
		return 0;
	}
	if (0 != (iRet = DnMid_WideCharToMultiByte((char*)stData.cFileName, buffer, len))){
		return 0;
	}
	return 1;
}

int cmmn_findClose(int search_handle)
{
	if (FindClose((HANDLE)search_handle) == FALSE){
		return -1;
	}
    return 0;
}

int cmmn_isExist(char *pathName, int isDir)
{
    return 0;
}

void cmmn_rmDir(char *pathName)
{
    
}

size_t cmmn_fileSize(char *pathName)
{
    return 0;
}

char* cmmn_fileName(char *pathName)
{
    return NULL;
}

int cmmn_extType(char *pathName)
{
    return 0;
}

char* cmmn_extTypeToStr(int extType)
{
    return NULL;
}