#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct node_t {
  void *I;

  struct node_t *next;
} *node;

typedef struct {
  node H;
  node T;
  int size;
  itemfree_t itemfree;
  int isel;
  node selected;
} list;

list *L = NULL;

/* Current amount of lists */
static int ln = 0;

static list struct_list_init(int size, itemfree_t itemfree) {
  list l;
  l.H = NULL;
  l.T = NULL;
  l.size = size;
  l.itemfree = itemfree;
  l.selected = NULL;
  l.isel = -1;
  return l;
}

static void realloc_L() {
  list *L2 = (list*) malloc( (ln + 1) * sizeof(list) );
  memcpy(L2, L, ln * sizeof(list));
  if ( L != NULL ) free(L);
  L = L2;
  ++ln;
}

static int get_ld() {
  int i;

  for ( i = 0; i < ln; ++i )
    if ( L[i].size == -1 )
      return i;

  realloc_L();
  return ln - 1;
}

int list_init(itemfree_t itemfree) {
  int ld = get_ld();

  if ( ld == -1 )
    return -1;

  L[ld] = struct_list_init(0, itemfree);

  return ld;
}

void list_free(int ld) {
  node tmp;

  if ( L[ld].size == -1 )
    return;

  while ( L[ld].H != NULL ) {
    tmp = L[ld].H->next;
    free(L[ld].H);
    L[ld].H = tmp;
  }

  L[ld] = struct_list_init(-1, NULL);
}

static node node_create(void *item, node next) {
  node tmp = (node) malloc( sizeof(struct node_t) );
  tmp->I = item;
  tmp->next = next;
  return tmp;
}

static void reset_cache(int ld) {
  L[ld].isel = 0;
  L[ld].selected = L[ld].H;
}

int list_insert(int ld, void *item) {
  if ( L[ld].H == NULL ) {
    L[ld].H = L[ld].T = node_create(item, NULL);
    reset_cache(ld);
  } else {
    L[ld].H = L[ld].H->next = node_create(item, NULL);
  }

  return ++L[ld].size;
}

int list_push(int ld, void *item) {
  if ( L[ld].size == 0 )
    return list_insert(ld, item);

  L[ld].H = node_create(item, L[ld].H);

  ++L[ld].isel;

  return ++L[ld].size;
}

void* list_pop(int ld) {
  void *item;
  node tmp;

  if ( L[ld].size == 0 )
    return NULL;

  item = L[ld].H->I;
  tmp = L[ld].H->next;
  free(L[ld].H);
  L[ld].H = tmp;

  if ( tmp == NULL )
    L[ld].H = NULL;

  --L[ld].size;

  if ( --L[ld].isel == -1 )
    reset_cache(ld);

  return item;
}

void* list_select(int ld, int i) {
  int j;

  if ( i >= L[ld].size )
    return NULL;

  if ( L[ld].isel > i ) {
    L[ld].isel = 0;
    L[ld].selected = L[ld].H;
  }

  for ( j = L[ld].isel; j < i; L[ld].selected = L[ld].selected->next, ++j );
  L[ld].isel = i;

  return L[ld].selected->I;
}

static int list_count_items(int ld) {
  int i = 0;
  node tmp = L[ld].H;

  for ( tmp = L[ld].H; tmp != NULL; tmp = tmp->next, ++i );

  return i;
}

int list_self_check(int ld) {
  if ( L[ld].H == NULL || L[ld].H == NULL )
    if ( L[ld].H != NULL || L[ld].H != NULL )
      return 0; // h null, t not null or vice versa

  if ( L[ld].size != list_count_items(ld) && L[ld].size != -1 )
    return 0;

  return 1;
}

void list_unload() {
  int i;

  if ( ln == 0 )
    return;

  for ( i = 0; i < ln; ++i )
    list_free(i);

  ln = 0;

  free(L);
  L = NULL;
}
