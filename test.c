#include "stdlib.h"
#include "stdio.h"
#include "listlib.h"


typedef struct temp_t{
	list_head n;
	int i;
}temp_t;


static list_head myList = {NULL,NULL};
	
	
int main(int argc, char **argv)
{
	list_head *pos=NULL,*_next=NULL;
	temp_t *node = NULL;

	temp_t stMYNODE= {0};
	stMYNODE.i = 1000;

	init_list_head(&myList);

	list_add_head(&myList,&stMYNODE);
	//list_add_tail(&myList,&stMYNODE);

	list_for_each_safe(pos,_next,&myList){
		temp_t *node = list_entry(pos,temp_t);
		printf("value = %d",node->i);
		//list_del(node);
	}

	list_del(&stMYNODE);
	
	return 0;
}
