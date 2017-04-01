#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "translate.h"

cell alloc_cell(){
  return malloc(sizeof(struct cell));
}

cell init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res){
  cell c = alloc_cell();
  c->name = NULL;
  c->arg1 = NULL;
  c->arg2 = NULL;
  c->res = NULL;
  c->next = NULL;
  c->def = def;
  if(name != NULL)
    c->name = strdup(name);
  if(arg1 != NULL)
    c->arg1 = strdup(arg1);
  if(arg2 != NULL)
    c->arg2 = strdup(arg2);
  if(res != NULL)
    c->res = strdup(res);
}

cell search_cell(cell c, char* name){
  cell tmp = c;
  while(tmp != NULL && strcmp(c->name, name) != 0){
    tmp = tmp->next;
  }
  return tmp;
}

void concat_cell(cell c1, cell c2){
  cell tmp = c1;
  while(tmp->next != NULL){
    tmp = tmp->next;
  }
  tmp->next = c2;
}

char* list_c3a[] = {"", "Pl", "Mo", "Mu", "And", "Or", "Ind", "Not", "Af", "Afc", "AfInd", "Sk", "Jp", "Jz", "St", "Param", "Call", "Ret"};

void display_cell(cell c){
  char* s = (c->name != NULL)?c->name:"";
  printf("%-12s:", s);
  char sep = (c->def == empty)?' ':':';
  if(sep != ' ')
    printf("%-6s%c", list_c3a[c->def], sep);
  s = (c->arg1 != NULL)?c->arg1:"";
  printf("%-12s%c", s, sep);
  s = (c->arg2 != NULL)?c->arg2:"";
  printf("%-12s%c", s, sep);
  s = (c->res != NULL)?c->res:"";
  printf("%-12s%c", s, sep);
}

void free_cell(cell c){
  cell tmp;
  while(c != NULL){
    free(c->name);
    free(c->arg1);
    free(c->arg2);
    free(c->res);
    tmp = c;
    c = c->next;
    free(tmp);
  }
}
