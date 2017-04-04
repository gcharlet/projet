#include "tree_abs.h"
#include "translate.h"

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
  int error;
} *heap;

typedef struct pile {
  cell c;
  struct pile* next;
} *pile;

extern env alloc_env();
extern heap alloc_heap();
extern pile alloc_pile();

extern env init_env(env e, char* name, type_exp type);
extern env init_env_list(tree s);
extern env init_env_int(env e, char* name);
extern heap init_heap(int space_address, int space_memory);
extern pile stack(cell c, pile daddy);
extern cell unstack(pile *p);

extern env concat_env(env e1, env e2);
extern void affect_env(env e, char* name, int value);
extern env search_env(env e, char* name);
extern int value_env(env e, char* name);
extern int new_array(heap h, int size);
extern int value_heap(heap h, int address, int indice);
extern int affect_heap(heap h, int address, int indice, int value);

extern int operation_pp(enum define def, int val1, int val2);
extern int value_env_pp(env G, env E, char* name);
extern int interp_pp(env *G, heap *H, tree s);
extern int interp_pp_code(env G, heap H, env E, tree code, tree lfunc);
extern int interp_pp_call(env G, heap H, env E, tree call, tree lfunc);

extern int interp_c3a(env *G, heap* H, list l);
extern int operation_c3a(enum c3a op, int val1, int val2);

extern void display_tab(heap H, int add, int depth, enum define def);
extern void display_env_heap(env G, heap H);

extern void free_env(env e);
extern void free_heap(heap h);

#endif
