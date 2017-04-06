#include "tree_abs.h"

#ifndef TRANSLATE_H
#define TRANSLATE_H

#define STRING_LENGTH 12

enum c3a {empty = 0, c_Pl, c_Mo, c_Mu, c_And, c_Or, c_Lt, c_Ind, c_Not, c_Af, c_Afc, c_AfInd, c_Sk, c_Jp, c_Jz, c_St, c_Param, c_Call, c_Ret};

typedef struct cell{
  char* name;
  enum c3a def;
  char* arg1;
  char* arg2;
  char* res;
  struct cell* next;
} *cell;

typedef struct list{
  cell first;
  cell end;
} *list;

extern cell alloc_cell();
extern list alloc_list();
extern char* alloc_string();

extern list init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res);
extern cell search_cell(list l, char* name);
extern void concat_list(list l1, list l2);

extern list translate_pp(tree s);
extern list translate_pp_function(tree function, int* et, int* ct, int* va, tree lfunc);
extern list translate_pp_code(tree code, int* et, int* ct, int* va, tree lfunc);
extern list translate_pp_operation(tree code, int* et, int* ct, int* va, tree lfunc);
extern list translate_pp_tab(tree tab, int depth, int* et, int* ct, int* va, tree lfunc);
extern list translate_pp_call(tree call, int* et, int* ct, int* va, tree lfunc);
extern tree argt_function_pp(tree lfunc, char* name);

extern void display_list(list l);

extern void free_list(list l);

#endif
