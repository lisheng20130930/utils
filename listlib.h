#ifndef __LIST_LIB_H__
#define __LIST_LIB_H__


typedef struct _list_head{
	struct _list_head *prev;
	struct _list_head *next;
}list_head;


#define init_list_head(p) do{(p)->prev = (p); (p)->next = (p);}while(0)
#define list_empty(p) ((p)->next == (p))
#define list_entry(p,type) ((type*)(p))

#define list_add_tail(p,n) do{((list_head*)n)->prev = (p)->prev; ((list_head*)n)->next = (p); (p)->prev->next = ((list_head*)n); (p)->prev = ((list_head*)n);}while(0)
#define list_add_head(p,n) do{((list_head*)n)->prev = (p); ((list_head*)n)->next = (p)->next; (p)->next->prev = ((list_head*)n); (p)->next = ((list_head*)n);}while(0)
#define list_del(n) do{((list_head*)n)->prev->next = ((list_head*)n)->next; ((list_head*)n)->next->prev = ((list_head*)n)->prev; ((list_head*)n)->prev = NULL,((list_head*)n)->next=NULL;}while(0)

#define list_for_each(n, p) for(n = (p)->next; n != (p); n = n->next)
#define list_for_each_safe(n,q,p) for(n = (p)->next, q = n->next; n != (p); n = q, q = n->next)


#endif
