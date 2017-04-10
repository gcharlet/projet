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
  if(l1->first != NULL)
    l1->end->next = l2->first;
  else
    l1->first = l2->first;
  l1->end = l2->end;
  free(l2);
}

list translate_pp(tree s){
  int et = 0, ct = 0, va = 0;
  list l = init_cell("", c_Afc, "1", NULL, "L_TAB#");
  concat_list(l, translate_pp_code(s->sons[2], &et, &ct, &va, s->sons[1]));
  concat_list(l, init_cell("", c_St, NULL, NULL, NULL));
  tree lfunc = s->sons[1];
  for(int i = 0; i < lfunc->nb_sons; i++)
    concat_list(l, translate_pp_function(lfunc->sons[i], &et, &ct, &va, s->sons[1]));
  return l;
}

list translate_pp_function(tree function, int* et, int* ct, int* va, tree lfunc){
  char* name = ((sign)function->sons[0])->name;
  list l = init_cell(name, c_Sk, NULL, NULL, NULL);
  concat_list(l, translate_pp_code(function->sons[2], et, ct, va, lfunc));
  concat_list(l, init_cell("", c_Ret, NULL, NULL, NULL));
  return l;
}

list translate_pp_code(tree code, int* et, int* ct, int* va, tree lfunc){
  char *name, *arg1, *arg2, *res;
  list l = NULL;
  cell jz, jp;
  switch(code->def)
    {
    case Pl:
    case Mo:
    case Mu:
    case Or:
    case And:
    case Lt:
      return translate_pp_operation(code, et, ct, va, lfunc);
      break;
    case Eq:
      l = translate_pp_operation(code, et, ct, va, lfunc);
      res = alloc_string();
      sprintf(res, "VA%d", (*va)++);
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Jz, l->end->res, NULL, NULL));
      jz = l->end;
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Afc, "0", NULL, res));
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Jp, NULL, NULL, NULL));
      jp = l->end;
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Afc, "1", NULL, res));
      jz->res = strdup(l->end->name);
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Sk, NULL, NULL, res));
      jp->res = strdup(name);
      free(res);
      free(name);
      return l;
      break;
    case Not:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      l = translate_pp_code(code->sons[0], et, ct, va, lfunc);
      arg1 = l->end->res;
      res = alloc_string();
      sprintf(res, "VA%d", (*va)++);
      concat_list(l, init_cell(name, c_Not, arg1, NULL, res));
      free(name);
      free(res);
      return l;
      break;
    case Call:
      return translate_pp_call(code, et, ct, va, lfunc);
      break;
    case NewAr:
      l = translate_pp_code(code->sons[1], et, ct, va, lfunc);
      arg2 = l->end->res;
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      arg1 = alloc_string();
      sprintf(arg1, "CT%d", (*ct)++);
      concat_list(l, init_cell(name, c_Af, arg1, "L_TAB#", NULL));
      res = alloc_string();
      sprintf(res, "CT%d", (*ct)++);
      sprintf(name, "ET%d", (*et)++);
      sprintf(res, "VA%d", (*va)++);
      concat_list(l, init_cell(name, c_Pl, "L_TAB#", arg2, res));
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Af, "L_TAB#", res, NULL));
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Sk, NULL, NULL, arg1));
      free(name);
      free(arg1);
      free(res);
      return l;
      break;
    case Tab:
      return translate_pp_tab(code, code->nb_sons, et, ct, va, lfunc);
      break;
    case Se:
      l = translate_pp_code(code->sons[0], et, ct, va, lfunc);
      concat_list(l, translate_pp_code(code->sons[1], et, ct, va, lfunc));
      return l;
      break;
    case Af:
      arg1 = code->sons[0];
      l = translate_pp_code(code->sons[1], et, ct, va, lfunc);
      arg2 = l->end->res;
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Af, arg1, arg2, NULL));
      free(name);
      return l;
      break;
    case AfTab:
      l = translate_pp_tab(code->sons[0], ((tree)code->sons[0])->nb_sons - 1, et, ct, va, lfunc);
      arg1 = l->end->res;
      concat_list(l, translate_pp_code(((tree)code->sons[0])->sons[((tree)code->sons[0])->nb_sons - 1], et, ct, va, lfunc));
      arg2 = l->end->res;
      concat_list(l, translate_pp_code(code->sons[1], et, ct, va, lfunc));
      res = l->end->res;
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_AfInd, arg1, arg2, res));
      free(name);
      return l;
      break;
    case Sk:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      l = init_cell(name, c_Sk, NULL, NULL, NULL);
      free(name);
      return l;
      break;
    case If:
      l = translate_pp_code(code->sons[0], et, ct, va, lfunc);
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      arg1 = l->end->res;
      concat_list(l, init_cell(name, c_Jz, arg1, NULL, NULL));
      jz = l->end;
      concat_list(l, translate_pp_code(code->sons[1], et, ct, va, lfunc));
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Jp, NULL, NULL, NULL));
      jp = l->end;
      concat_list(l, translate_pp_code(code->sons[2], et, ct, va, lfunc));
      jz->res = strdup(jp->next->name);
      sprintf(name, "ET%d", (*et)++);
      jp->res = strdup(name);
      concat_list(l, init_cell(name, c_Sk, NULL, NULL, NULL));
      free(name);
      return l;
      break;
    case Wh:
      l = translate_pp_code(code->sons[0], et, ct, va, lfunc);
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      arg1 = l->end->res;
      concat_list(l, init_cell(name, c_Jz, arg1, NULL, NULL));
      jz = l->end;
      concat_list(l, translate_pp_code(code->sons[1], et, ct, va, lfunc));
      sprintf(name, "ET%d", (*et)++);
      concat_list(l, init_cell(name, c_Jp, NULL, NULL, l->first->name));
      sprintf(name, "ET%d", (*et)++);
      jz->res = strdup(name);
      concat_list(l, init_cell(name, c_Sk, NULL, NULL, NULL));
      free(name);
      return l;
      break;
    case Val:
      name = alloc_string();
      sprintf(name, "ET%d", (*et)++);
      val v = code->sons[0];
      if(v->def == Var){
	l = init_cell(name, c_Sk, NULL, NULL, v->param.name);
	free(name);
	return l;
      }
      arg1 = alloc_string();
      res = alloc_string();
      sprintf(arg1, "%d", v->param.val);
      sprintf(res, "CT%d", (*ct)++);
      l = init_cell(name, c_Afc, arg1, NULL, res);
      free(name);
      free(arg1);
      free(res);
      return l;
      break;
    }
}

list translate_pp_operation(tree code, int* et, int* ct, int* va, tree lfunc){
  enum c3a def;
  char* name = alloc_string();
  char* res = alloc_string();
  switch(code->def)
    {
    case Pl:
      def = c_Pl;
      break;
    case Mo:
    case Eq:
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
    case Lt:
      def = c_Lt;
      break;
    }
  list l = translate_pp_code(code->sons[0], et, ct, va, lfunc);
  char* arg1 = l->end->res;
  concat_list(l, translate_pp_code(code->sons[1], et, ct, va, lfunc));
  char* arg2 = l->end->res;
  sprintf(name, "ET%d", (*et)++);
  sprintf(res, "VA%d", (*va)++);
  concat_list(l, init_cell(name, def, arg1, arg2, res));
  free(name);
  free(res);
  return l;
}

list translate_pp_tab(tree tab, int depth, int* et, int* ct, int* va, tree lfunc){
  char *name, *arg1, *arg2, *res;
  list l = NULL;
  name = alloc_string();
  res = alloc_string();
  sprintf(name, "ET%d", (*et)++);
  arg1 = tab->sons[0];
  l = init_cell(name, c_Sk, NULL, NULL, arg1);
  for(int i = 1; i < depth; i++){
    concat_list(l, translate_pp_code(tab->sons[i], et, ct, va, lfunc));
    sprintf(name, "ET%d", (*et)++);
    sprintf(res, "VA%d", (*va)++);
    arg2 = l->end->res;
    concat_list(l, init_cell(name, c_Ind, arg1, arg2, res));
    arg1 = l->end->res;
  }
  free(name);
  free(res);
  return l;
}

list translate_pp_call(tree call, int* et, int* ct, int* va, tree lfunc){
  char *name, *res, *arg2;
  list l = alloc_list();
  l->first = NULL;
  l->end = NULL;
  tree args = call->sons[1];
  tree argt = argt_function_pp(lfunc, call->sons[0]);
  name = alloc_string();
  for(int i = args->nb_sons-1; i >= 0 ; i--){
    concat_list(l, translate_pp_code(args->sons[i], et, ct, va, lfunc));
    sprintf(name, "ET%d", (*et)++);
    concat_list(l, init_cell(name, c_Param, ((var)argt->sons[i])->name, l->end->res, NULL));
  }
  res = alloc_string();
  sprintf(res, "VA%d", (*va)++);
  sprintf(name, "ET%d", (*et)++);
  arg2 = alloc_string();
  sprintf(arg2, "%d", args->nb_sons);
  concat_list(l, init_cell(name, c_Call, call->sons[0], arg2, res));
  free(name);
  free(res);
  free(arg2);
  return l;
}

tree argt_function_pp(tree lfunc, char* name){
  sign v = NULL;
  for(int i = 0; i < lfunc->nb_sons; i++){
    v = ((tree)lfunc->sons[i])->sons[0];
    if(strcmp(name, v->name) == 0)
      break;
  }
  if(v == NULL)
    return NULL;
  return v->argt;
}

char* list_c3a[] = {"", "Pl", "Mo", "Mu", "And", "Or", "Lt", "Ind", "Not", "Af", "Afc", "AfInd", "Sk", "Jp", "Jz", "St", "Param", "Call", "Ret"};

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
