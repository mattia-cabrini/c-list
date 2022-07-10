#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef void(*itemfree_t)(void*);

extern int   list_init(itemfree_t);
extern void  list_free(int);
extern int   list_push(int, void*);
extern int   list_insert(int, void*);
extern void* list_select(int, int);
extern void* list_pop(int);
extern void  list_unload();

#endif // LIST_H_INCLUDED
