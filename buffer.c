#include "buffer.h"
#include "libos.h"

#define BUFFER_DEFUALT_SIZE			(512)

/////////////////////////////////////////////////////////////////
void buffer_init(buffer_t *des)
{
	if(des == NULL){
		return;
	}		
	des->data = NULL;
	des->max = 0;
	des->size = 0;
}

void buffer_deinit(buffer_t *des)
{
	if(des == NULL){
		return;
	}

	if(des->data)
		free(des->data);
	des->data = NULL;
	des->max = 0;
	des->size = 0;
}

void buffer_append(buffer_t *des, char* str, int size)
{
	int new_size;
	int i;
	
	if(des==NULL||size==0){
		return;
	}
	new_size = size + des->size;
	if(new_size >= des->max){
		int _SIZE = __max(new_size,des->max+BUFFER_DEFUALT_SIZE);
		char* temp = (char*)malloc(_SIZE);
		memset(temp,0x00,_SIZE);
		for(i = 0; i<des->size; i++){
			temp[i] = des->data[i];
		}
		if(str){
			for(i = 0; i < size; i++){
				temp[des->size + i] = str[i];
			}
		}
		else{
			for(i = 0; i < size; i++){
				temp[des->size + i] = 0;
			}
		}

		des->max = _SIZE;
		des->size = new_size;
		if(des->data)
			free(des->data);
		des->data = temp;
	}
	else{
		if(str){
			for(i = 0; i < size; i++){
				des->data[des->size++] = str[i];
			}
		}
		else{
			for(i = 0; i < size; i++){
				des->data[des->size++] = 0;
			}
		}
	}
}

int buffer_length(buffer_t *des)
{
	if(des==NULL){
		return 0;
	}
	return des->size;
}
