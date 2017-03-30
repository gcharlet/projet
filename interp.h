#include "tree_abs.h"

#ifndef INTERP_H
#define INTERP_H

typedef struct env {
  char* name;
  type_exp type;
  int value;
  struct env* next;
} *env;

typedef struct heap {
  int* address;
  int* size;
  int* memory;
  int last_memory;
  int last_address;
} *heap;

extern env alloc_env();
extern heap alloc_heap();

extern env init_env(env e, char* name, type_exp type);
extern env init_env_list(tree s);
extern heap init_heap(int space_address, int space_memory);

extern env concat_env(env e1, env e2);
extern void affect_env(env e, char* name, int value);
extern env search_env(env e, char* name);
extern int value_env(env e, char* name);
extern int new_array(heap h, int size);
extern int value_heap(heap h, int address, int indice);
extern int affect_heap(heap h, int address, int indice, int value);

extern void free_env();
extern void free_heap();

#endif
