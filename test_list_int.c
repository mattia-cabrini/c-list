#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "c-test/test.h"

extern int list_self_check(int);
int* get_int(int);

void test001_init(test_r *T);
void test002_init_2(test_r *T);
void test003_push(test_r *T);
void test004_insert(test_r *T);
void test005_pop(test_r *T);
void test006_select(test_r *T);
void test007_select_2(test_r *T);
void test008_select_after_pop(test_r *T);
void test009_reuse_symbol(test_r *T);
void test010_unload(test_r *T);

int main(int argc, char **argv) {
  test_r T;
  int iok = 0, iko = 0;

  test_add(test001_init);
  test_add(test002_init_2);
  test_add(test003_push);
  test_add(test004_insert);
  test_add(test005_pop);
  test_add(test006_select);
  test_add(test007_select_2);
  test_add(test008_select_after_pop);
  test_add(test009_reuse_symbol);
  test_add(test010_unload);

  test_run();

  while ( test_pop(&T) != -1 ) {
    if ( T.success ) {
      fprintf(stdout, "OK | %s\n", T.error);
      iok += 1;

    } else {
      fprintf(stderr, "KO | %s\n", T.error);
      iko += 1;
    }
  }

  printf("\nTests ok: %d/%d\n", iok, iok + iko);


  return 0;
}

int* get_int(int i) {
  int *j = malloc( sizeof(int) );
  *j = i;
  return j;
}

void test001_init(test_r *T) {
  int ld = list_init(free);

  if ( ld == -1 ) {
    *T = test_r_init(0, "001 | Cannot create list");
    return;
  }

  list_free(ld);
  *T = test_r_init(1, "001 | Test ok.");
}

void test002_init_2(test_r *T) {
  int ld = list_init(free);
  int ld2 = list_init(free);

  if ( ld == -1 ) {
    *T = test_r_init(0, "002 | Cannot create list 1");
    return;
  }

  if ( ld2 == -1 ) {
    *T = test_r_init(0, "002 | Cannot create list 2");
    return;
  }

  if ( ld + 1 != ld2 ) {
    printf("");
    *T = test_r_init(0, "002 | Wrong sequential number");
    return;
  }

  list_free(ld);
  list_free(ld2);
  *T = test_r_init(1, "002 | Test ok.");
}

void test003_push(test_r *T) {
  int ld = list_init(free), cur;

  if ( ld == -1 ) {
    *T = test_r_init(0, "003 | Cannot create list");
    return;
  }

  cur = list_push(ld, get_int(0));

  if ( cur != 1 ) {
    *T = test_r_init(0, "003 | List push returned wrong size");
    return;
  }

  cur = list_push(ld, get_int(0));

  if ( cur != 2 ) {
    *T = test_r_init(0, "003 | List push returned wrong size");
    return;
  }

  list_free(ld);
  *T = test_r_init(1, "003 | Test ok.");
}

void test004_insert(test_r *T) {
  int ld = list_init(free), cur;

  if ( ld == -1 ) {
    *T = test_r_init(0, "004 | Cannot create list");
    return;
  }

  cur = list_insert(ld, get_int(0));

  if ( cur != 1 ) {
    *T = test_r_init(0, "004 | List push returned wrong size");
    return;
  }

  cur = list_insert(ld, get_int(0));

  if ( cur != 2 ) {
    *T = test_r_init(0, "004 | List push returned wrong size");
    return;
  }

  list_free(ld);
  *T = test_r_init(1, "004 | Test ok.");
}

void test005_pop(test_r *T) {
  int ld = list_init(free), i, *j;

  if ( ld == -1 ) {
    *T = test_r_init(0, "005 | Cannot create list");
    return;
  }

  for ( i = 0; i < 10; ++i ) {
    if ( list_push(ld, get_int(i)) != i + 1 ) {
      *T = test_r_init(0, "005 | List push returned wrong size");
      list_free(ld);
      return;
    }
  }

  for ( i = 0; i < 10; ++i ) {
    j = list_pop(ld);
    if ( j == NULL ) {
      *T = test_r_init(0, "005 | List pop returned NULL");
      list_free(ld);
      return;
    }
    if ( *j != 9 - i ) {
      *T = test_r_init(0, "005 | List pop returned wrong value");
      list_free(ld);
      free(j);
      return;
    }
    free(j);
  }

  list_free(ld);
  *T = test_r_init(1, "005 | Test ok.");
}

void test006_select(test_r *T) {
  int ld = list_init(free), i, *j;

  if ( ld == -1 ) {
    *T = test_r_init(0, "006 | Cannot create list");
    return;
  }

  for ( i = 0; i < 10; ++i ) {
    if ( list_insert(ld, get_int(i)) != i + 1 ) {
      *T = test_r_init(0, "006 | List insert returned wrong size");
      list_free(ld);
      return;
    }
  }

  for ( i = 0; i < 10; ++i ) {
    j = list_select(ld, i);
    if ( j == NULL ) {
      *T = test_r_init(0, "006 | List select returned NULL");
      list_free(ld);
      return;
    }
    if ( *j != i ) {
      *T = test_r_init(0, "006 | List select returned wrong value");
      list_free(ld);
      return;
    }
  }

  list_free(ld);
  *T = test_r_init(1, "006 | Test ok.");
}

void test007_select_2(test_r *T) {
  int ld = list_init(free), i, *j;

  if ( ld == -1 ) {
    *T = test_r_init(0, "007 | Cannot create list");
    return;
  }

  for ( i = 0; i < 10; ++i ) {
    if ( list_push(ld, get_int(i)) != i + 1 ) {
      *T = test_r_init(0, "007 | List push returned wrong size");
      list_free(ld);
      return;
    }

    if ( !list_self_check(ld) ) {
      *T = test_r_init(0, "007 | List failed self check");
      list_free(ld);
      return;
    }
  }

  for ( i = 9; i > -1; --i ) {
    j = list_select(ld, i);
    if ( j == NULL ) {
      *T = test_r_init(0, "007 | List select returned NULL");
      list_free(ld);
      return;
    }
    if ( *j != 9 - i ) {
      *T = test_r_init(0, "007 | List select returned wrong value");
      list_free(ld);
      return;
    }
  }

  list_free(ld);

  if ( !list_self_check(ld) ) {
    *T = test_r_init(0, "007 | List failed self check");
    list_free(ld);
    return;
  }

  *T = test_r_init(1, "007 | Test ok.");
}

void test008_select_after_pop(test_r *T) {
  int ld = list_init(free), *j, *k;

  if ( ld == -1 ) {
    *T = test_r_init(0, "008 | Cannot create list");
    return;
  }

  list_push(ld, get_int(3));
  list_push(ld, get_int(2));
  list_push(ld, get_int(1));

  if ( !list_self_check(ld) ) {
    *T = test_r_init(0, "008 | List failed self check");
    list_free(ld);
    return;
  }

  list_pop(ld);

  if ( !list_self_check(ld) ) {
    *T = test_r_init(0, "008 | List failed self check");
    list_free(ld);
    return;
  }

  j = list_select(ld, 0);
  k = list_select(ld, 1);

  if ( *j != 2 || *k != 3 ) {
    *T = test_r_init(0, "008 | List select returned wrong value");
    list_free(ld);
    return;
  }

  free(j);
  free(k);

  list_free(ld);
  *T = test_r_init(1, "008 | Test ok.");
}

void test009_reuse_symbol(test_r *T) {
  int ld, ld2;

  if ( (ld = list_init(NULL)) == -1 ) {
    *T = test_r_init(0, "009 | Cannot initialise list");
    return;
  }

  list_free(ld);

  if ( (ld2 = list_init(NULL)) == -1 ) {
    *T = test_r_init(0, "009 | Cannot initialise list");
    return;
  }

  list_free(ld2);

  if ( ld != ld2 ) {
    *T = test_r_init(0, "009 | Not reusing descriptors");
    return;
  }

  *T = test_r_init(1, "009 | Test ok.");
}

void test010_unload(test_r *T) {
  int ld, ld2;

  list_unload();

  if ( (ld = list_init(NULL)) == -1 ) {
    *T = test_r_init(0, "010 | Cannot initialise list");
    return;
  }

  list_free(ld);

  if ( (ld2 = list_init(NULL)) == -1 ) {
    *T = test_r_init(0, "010 | Cannot initialise list");
    return;
  }

  list_free(ld2);

  if ( ld != ld2 ) {
    *T = test_r_init(0, "010 | Not reusing descriptors");
    return;
  }

  *T = test_r_init(1, "010 | Test ok.");
}
