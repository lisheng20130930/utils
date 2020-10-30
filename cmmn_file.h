#ifndef CMMN_FILE_H
#define CMMN_FILE_H


#define CMMN_FILE_MAX_PATH (1024)


int cmmn_findFirstFile(const char* name, char* buffer, int len);
int cmmn_findNextFile(int search_handle, char* buffer, int len);
int cmmn_findClose(int search_handle);

bool cmmn_isFileExist(char *pathName, int isDir);

void cmmn_rmDir(char *pathName);

size_t cmmn_fileSize(char *pathName);

char* cmmn_fileName(char *pathName);

int cmmn_extType(char *pathName);
char* cmmn_extTypeToStr(int extType);


FILE* cmmn_openFile(char *pathName, char *mode);
void cmmn_closeFile(FILE *pFile);

unsigned int cmmn_fileWriteTo(FILE *pFile, int seek_pos, char *buffer, unsigned int len);
unsigned int cmmn_fileReadFrom(FILE *pFile, int seek_pos, char *buffer, unsigned int len);

unsigned int cmmn_fileWrite(FILE *pFile, char *buffer, unsigned int len);
unsigned int cmmn_fileRead(FILE *pFile, char *buffer, unsigned int len);


bool file2buffer(char *pszFileName, char **ppszBuff, int *piLen);
bool buffer2file(char *buffer, int len, char *pszFileName);


#endif