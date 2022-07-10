#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct node_t {
  void *I;

  struct node_t *next;
} *node;

/* Heads, Tails and sizes */
static node *H = NULL;
static node *T = NULL;
static int  *S = NULL;

/* Item Frees */
static itemfree_t *IF = NULL;

/* Current amount of lists */
static int ln = 0;

/* Cache for select */
static node *cSel = NULL;
static int  *iSel = NULL;

static void* realloc_X(void *X, int DIM, size_t size) {
  int i;
  char *X2;

  X2 = (char* ) malloc( (DIM + 1) * size );

  if ( DIM != 0 ) {
    memcpy(X2, X, DIM * size);
    free(X);
  }

  return X2;
}

static void realloc_L() {
  H = realloc_X(H, ln, sizeof(struct node_t));
  T = realloc_X(T, ln, sizeof(struct node_t));
  S = realloc_X(S, ln, sizeof(int));

  IF = realloc_X(IF, ln, sizeof(itemfree_t));

  cSel = realloc_X(cSel, ln, sizeof(struct node_t));
  iSel = realloc_X(iSel, ln, sizeof(int));

  H[ln] = NULL;
  T[ln] = NULL;
  S[ln] = 0;
  IF[ln] = NULL;
  cSel[ln] = NULL;
  iSel[ln] = 0;

  ++ln;
}

static int get_ld() {
  int i;

  for ( i = 0; i < ln; ++i )
    if ( S[i] == -1 )
      return i;

  realloc_L();
  return ln - 1;
}

int list_init(itemfree_t itemfree) {
  int ld = get_ld();

  if ( ld == -1 )
    return -1;

  IF[ld] = itemfree;
  S[ld] = 0;

  return ld;
}

void list_free(int ld) {
  node tmp;

  if ( S[ld] == -1 )
    return;

  while ( H[ld] != NULL ) {
    tmp = H[ld]->next;
    free(H[ld]);
    H[ld] = tmp;
  }

  S[ld] = -1;
  T[ld] = NULL;
  IF[ld] = NULL;
  iSel[ld] = -1;
  cSel[ld] = NULL;
}

static node node_create(void *item, node next) {
  node tmp = (node) malloc( sizeof(struct node_t) );
  tmp->I = item;
  tmp->next = next;
  return tmp;
}

static void reset_cache(int ld) {
  iSel[ld] = 0;
  cSel[ld] = H[ld];
}

int list_insert(int ld, void *item) {
  if ( H[ld] == NULL ) {
    H[ld] = T[ld] = node_create(item, NULL);
    reset_cache(ld);
  } else {
    T[ld] = T[ld]->next = node_create(item, NULL);
  }

  return ++S[ld];
}

int list_push(int ld, void *item) {
  if ( S[ld] == 0 )
    return list_insert(ld, item);

  H[ld] = node_create(item, H[ld]);

  ++iSel[ld];

  return ++S[ld];
}

void* list_pop(int ld) {
  void *item;
  node tmp;

  if ( S[ld] == 0 )
    return NULL;

  item = H[ld]->I;
  tmp = H[ld]->next;
  free(H[ld]);
  H[ld] = tmp;

  if ( tmp == NULL )
    T[ld] = NULL;

  --S[ld];

  if ( --iSel[ld] == -1 )
    reset_cache(ld);

  return item;
}

void* list_select(int ld, int i) {
  int j;

  if ( i >= S[ld] )
    return NULL;

  if ( iSel[ld] > i ) {
    iSel[ld] = 0;
    cSel[ld] = H[ld];
  }

  for ( j = iSel[ld]; j < i; cSel[ld] = cSel[ld]->next, ++j );
  iSel[ld] = i;

  return cSel[ld]->I;
}

static int list_count_items(int ld) {
  int i = 0;
  node tmp = H[ld];

  for ( tmp = H[ld]; tmp != NULL; tmp = tmp->next, ++i );

  return i;
}

int list_self_check(int ld) {
  if ( H[ld] == NULL || T[ld] == NULL )
    if ( H[ld] != NULL || T[ld] != NULL )
      return 0; // h null, t not null or vice versa

  if ( S[ld] != list_count_items(ld) && S[ld] != -1 )
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

  free(H);
  free(T);
  free(S);
  free(IF);
  free(iSel);
  free(cSel);
}
