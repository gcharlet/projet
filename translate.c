#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "translate.h"

cell alloc_cell(){
  return malloc(sizeof(struct cell));
}

list alloc_list(){
  return malloc(sizeof(struct list));
}

char* alloc_string(){
  return malloc(sizeof(char) * STRING_LENGTH);
}

list init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res){
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
  list l = alloc_list();
  l->first = c;
  l->end = c;
  return l;
}

cell search_cell(list l, char* name){
  cell tmp = l->first;
  while(tmp != NULL && strcmp(tmp->name, name) != 0){
    tmp = tmp->next;
  }
  return tmp;
}

void concat_list(list l1, list l2){
  l1->end->next = l2->first;
  l1->end = l2->end;
  free(l2);
}

list translate_pp(tree s){
  int et = 0, ct = 0, va = 0;
  list l = translate_pp_code(s->sons[2], &et, &ct, &va);
  tree lfunc = s->sons[1];
  for(int i = 0; i < lfunc->nb_sons; i++)
    concat_list(l, translate_pp_function(lfunc->sons[i], &et, &ct, &va));
  return l;
}

list translate_pp_function(tree function, int* et, int* ct, int* va){
  return NULL;
}

list translate_pp_code(tree code, int* et, int* ct, int* va){
  char *name, *arg1, *arg2, *res;
  list l = NULL;
  switch(code->def)
    {
    case Pl:
    case Mo:
    case Mu:
    case Or:
    case And:
      return translate_pp_operation(code, et, ct, va);
      break;
    case Eq:
    case Lt:
      break;
    case Not:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      l = translate_pp_code(code->sons[0], et, ct, va);
      arg1 = strdup(l->end->res);
      res = alloc_string();
      sprintf(res, "VA%d", (*va)++);
      concat_list(l, init_cell(name, c_Not, arg1, NULL, res));
      return l;
      break;
    case Call:
      break;
    case NewAr:
      break;
    case Tab:
      break;
    case Se:
      l = translate_pp_code(code->sons[0], et, ct, va);
      concat_list(l, translate_pp_code(code->sons[1], et, ct, va));
      return l;
      break;
    case Af:
      arg1 = strdup(code->sons[0]);
      l = translate_pp_code(code->sons[1], et, ct, va);
      arg2 = strdup(l->end->res);
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Af, arg1, arg2, NULL));
      return l;
      break;
    case AfTab:
      break;
    case Sk:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      return init_cell(name, c_Sk, NULL, NULL, NULL);
      break;
    case If:
      break;
    case Wh:
      break;
    case Largs:
      break;
    case Val:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      val v = code->sons[0];
      if(v->def == Var){
	return init_cell(name, c_Sk, NULL, NULL, strdup(v->param.name));
      }
      arg1 = alloc_string();
      res = alloc_string();
      sprintf(arg1, "%d", v->param.val);
      sprintf(res, "CT%d", (*ct)++);
      return init_cell(name, c_Afc, arg1, NULL, res);
      break;
    }
}

list translate_pp_operation(tree code, int* et, int* ct, int* va){
  enum c3a def;
  char* name = alloc_string();
  char* res = alloc_string();
  switch(code->def)
    {
    case Pl:
      def = c_Pl;
      break;
    case Mo:
      def = c_Mo;
      break;
    case Mu:
      def = c_Mu;
      break;
    case Or:
      def = c_Or;
      break;
    case And:
      def = c_And;
      break;
    }
  list l = translate_pp_code(code->sons[0], et, ct, va);
  char* arg1 = strdup(l->end->res);
  concat_list(l, translate_pp_code(code->sons[1], et, ct, va));
  char* arg2 = strdup(l->end->res);
  sprintf(name, "ET%d", (*et)++);
  sprintf(res, "VA%d", (*va)++);
  concat_list(l, init_cell(name, def, arg1, arg2, res));
  return l;
}

char* list_c3a[] = {"", "Pl", "Mo", "Mu", "And", "Or", "Ind", "Not", "Af", "Afc", "AfInd", "Sk", "Jp", "Jz", "St", "Param", "Call", "Ret"};

void display_list(list l){
  cell c = l->first;
  while(c != NULL){
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
    printf("%-12s", s);
    c = c->next;
    printf("\n");
  }
}

void free_list(list l){
  cell c = l->first;
  cell tmp;
  while(c != NULL){
    if(c->name != NULL)
      free(c->name);
    if(c->arg1 != NULL)
      free(c->arg1);
    if(c->arg2 != NULL)
      free(c->arg2);
    if(c->res != NULL)
      free(c->res);
    tmp = c;
    c = c->next;
    free(tmp);
  }
  free(l);
}
