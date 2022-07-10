***

Table of contents

- [C-List](#c-list)
- [Build and Test](#build-and-test)
  - [Buld](#buld)
  - [Include](#include)
  - [Test](#test)
- [Usage](#usage)
  - [Abstract Data Type Utilities](#abstract-data-type-utilities)
  - [Initialization and Handler](#initialization-and-handler)
  - [Insert an Item](#insert-an-item)
    - [Push](#push)
    - [Insert](#insert)
  - [Pop an Item](#pop-an-item)
  - [Select an Item](#select-an-item)
  - [Other Functions](#other-functions)
- [Data Ownership](#data-ownership)
- [Function Complexity](#function-complexity)

***

# C-List

`c-list` is a Linked List library written in the C programming language made to provide the means to build and use one or many linked lists (from now on, just "lists").
Made as a general purpose library, `c-list` is implemented in order to handle any type of item.

# Build and Test

## Buld 

The list implementation is contained in the file `list.c` and its header in the file `list.h`.

The repository provides a makefile which can be used to compile a static library.

This makefile relies on the [GCC](https://gcc.gnu.org/) compiler.

``` sh
make list
```

The static library `list.a` will be placed under directory `x`.

## Include

To use c-list, it is sufficient to include `list.h` in the project and to link `list.a` during the linking process.

## Test

Besides the building process, `Makefile` also gives the means to run automated tests for the library.

The command:

```sh
make test
```

will compile and run the complete set.

# Usage

## Abstract Data Type Utilities 

In order to work with any data structure, the heap needs to know how to free an item.

```C
void itemfree(void *item) {
    // ...
}
```

## Initialization and Handler

```C
#include "list.h"

// ...

int ld = list_init(itemfree);
```

`ld` will be valorized with:

- `-1` if not list ha been initialized;
- `>= 0` if the list has been initialized.

`ld` is necessary to work on the list. We'll call this variable *the list handler*.

## Insert an Item

There are two ways to insert an item: *to push* and *to insert*.

### Push

Pushing an item onto the list means to place the item right on top of the list:

```C
int s = list_push(ld, item);
```

`s` is the size of the list.

### Insert

```C
int s = list_insert(ld, item);
```

`s` is the size of the list.

## Pop an Item

The front element of the list can be easily extracted using the *pop* feature:

```C
item *i = list_pop(ld);
```

## Select an Item

Any item in the list can be selected with the *select* feature:

```C
item *i = list_select(ld, n);
```

Items indexes start from `0`.

## Other Functions

```C
/* Frees a list */
list_free(ld);

/* Frees all lists */
list_unload();
```

# Data Ownership

|Function|Ownership|
|-:|:-|
int list_init   | does not matter with ownership
int list_push   | transfers ownership to the list library 
int list_insert | transfers ownership to the list library 
void* list_select | does not transfer ownership
void* list_pop | transfers ownership to the caller
void list_free | does not matter with ownership
void list_unload | does not matter with ownership

# Function Complexity

|Function|Complexity|
|-:|:-:|
int list_init     | *1*
int list_push     | *1*
int list_insert   | *1*
void* list_select | *n* or *1*\*
void* list_pop    | *1*
void list_free    | *n*
void list_unload  | *N = n0 + n1 + n2 + ...*

\* `list_select` has linear complexity for random indexes. If accesses are made from `0` to `n - 1`, complexity bacame *1*.
