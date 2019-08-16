#ifndef URL_ENCODE_H
#define URL_ENCODE_H



char *url_encode(char const *s, int len, int *new_length);
int url_decode(char *str, int len);


#endif