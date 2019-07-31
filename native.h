#ifndef NATIVE_H
#define NATIVE_H



typedef void (*native_callback)(int code, char *result);


void nb_routerCallback(char *cbName, int code, char *result);
void nb_callRouter(char *method, char *reqStr, native_callback cb);


#endif