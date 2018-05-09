#ifndef __BUFFER_H__
#define __BUFFER_H__

typedef struct _buffer_t{
	char* data;
	int	size;
	int	max;
}buffer_t;

void buffer_init(buffer_t* des);
void buffer_deinit(buffer_t *des);
void buffer_append(buffer_t *des, char* str, int size);
int buffer_length(buffer_t *des);


#endif

