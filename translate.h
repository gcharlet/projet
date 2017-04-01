#include "tree_abs.h"

#ifndef TRANSLATE_H
#define TRANSLATE_H

enum c3a {empty = 0, c_Pl, c_Mo, c_Mu, c_And, c_Or, c_Ind, c_Not, c_Af, c_Afc, c_AfInd, c_Sk, c_Jp, c_Jz, c_St, c_Param, c_Call, c_Ret};

typedef struct cell{
  char* name;
  enum c3a def;
  char* arg1;
  char* arg2;
  char* res;
  struct cell* next;
} *cell;

extern cell alloc_cell();

extern cell init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res);
extern cell search_cell(cell c, char* name);
extern void concat_cell(cell c1, cell c2);

extern void display_cell(cell c);

extern void free_cell(cell c);

#endif
