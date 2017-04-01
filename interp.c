#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"

env alloc_env(){
  return malloc(sizeof(struct env));
}

heap alloc_heap(){
  return malloc(sizeof(struct heap));
}

env init_env(env e, char* name, type_exp type){
  if(search_env(e, name) != NULL)
    return e;
  env tmp = alloc_env();
  tmp->name = strdup(name);
  tmp->type = copy_type_exp(type);
  tmp->value = 0;
  tmp->next = NULL;
  tmp->next = e;
  return tmp;
}

env init_env_list(tree s){
  if(s->def != Lvart)
    return NULL;
  env e = NULL;
  for(int i = 0; i < s->nb_sons; i++){
    var v = (var)s->sons[i];
    e = init_env(e, v->name, v->type);
  }
  return e;
}
  
heap init_heap(int space_address, int space_memory){
  heap h= alloc_heap();
  h->address = malloc(sizeof(int) * space_address);
  h->size = malloc(sizeof(int) * space_address);
  h->memory = malloc(sizeof(int) * space_memory);
  h->last_memory = 1;
  h->last_address = 1;
  h->error = 0;
  for(int i = 0; i < space_address; i++){
    h->address[i] = 0;
    h->size[i] = 0;
  }
  for(int i = 0; i < space_memory; i++)
    h->memory[i] = 0;
  return h;
}

env concat_env(env e1, env e2){
  if(e1 == NULL)
    return e2;
  if(e2 == NULL)
    return e1;
  env e = e2;
  while(e->next != NULL)
    e = e->next;
  e->next = e1;
  return e2;
}

void affect_env(env e, char* name, int value){
  env p = search_env(e, name);
  if(p != NULL)
    p->value = value;
}

env search_env(env e, char* name){
  env p = e;
  while(p != NULL && strcmp(p->name, name) != 0)
    p = p->next;
  return p;
}

int value_env(env e, char* name){
  env p = search_env(e, name);
  if(p != NULL)
    return p->value;
  return 0;
}

int new_array(heap h, int size){
  int i = h->last_address;
  h->address[i] = h->last_memory;
  h->size[i] = size;
  h->last_address += 1;
  h->last_memory += size;
  return i;
}

int value_heap(heap h, int address, int indice){
  int i = h->address[address];
  if(i == 0 || indice >= h->size[address]){
    printf("erreur de lecture dans un tableau\n");
    h->error += 1;
    return 0;
  }
  return h->memory[i+indice];
}

int affect_heap(heap h, int address, int indice, int value){
  if(h->address[address] == 0 || indice >= h->size[address]){
    printf("erreur d'ecriture dans un tableau\n");
    h->error += 1;
    return -1;
  }
  h->memory[h->address[address] + indice] = value;
  return 0;
}

int operation_pp(enum define def, int val1, int val2){
  switch (def)
    {
    case Pl:
      return val1 + val2;
    case Mo:
      return val1 - val2;
    case Mu:
      return val1 * val2;
    case Or:
      return (val1 || val2)?1:0;
    case And:
      return (val1 && val2)?1:0;
    case Lt:
      return (val1 < val2)?1:0;
    case Eq:
      return (val1 == val2)?1:0;
    }
}

int value_env_pp(env G, env E, char* name){
  if(E != NULL){
    env v = search_env(E, name);
    if(v != NULL)
      return v->value;
  }
  return value_env(G, name);
}

int interp_pp(env *G, heap *H, tree s){
  *G = init_env_list(s->sons[0]);
  *H = init_heap(1000, 10000);
  interp_pp_code(*G, *H, NULL, s->sons[2], s->sons[1]);
}

int interp_pp_code(env G, heap H, env E, tree code, tree lfunc){
  switch (code->def)
    {
    case Val:
      switch (((val)code->sons[0])->def)
	{
	case Bool:
	case Int:
	  return ((val)code->sons[0])->param.val;
	case Var:
	  return value_env_pp(G, E, ((val)code->sons[0])->param.name);
	}
      break;
    case Call:
      return interp_pp_call(G, H, E, code, lfunc);
      break;
    case NewAr:
      return new_array(H, interp_pp_code(G, H, E, code->sons[1], lfunc));
      break;
    case Tab:
      ;
      int add = value_env_pp(G, E, code->sons[0]);
      for(int i = 1; i < code->nb_sons; i++)
	add = value_heap(H, add, interp_pp_code(G, H, E, code->sons[i], lfunc));
      return add;
      break;
    case Af:
      ;
      env p = NULL;
      if(E != NULL)
	p = search_env(E, code->sons[0]);
      if(p == NULL)
	p = search_env(G, code->sons[0]);
      p->value = interp_pp_code(G, H, E, code->sons[1], lfunc);
      break;
    case AfTab:
      ;
      tree tab = code->sons[0];
      int addr = value_env_pp(G, E, tab->sons[0]);
      for(int i = 1; i < (code->nb_sons-1); i++)
	addr = value_heap(H, add, interp_pp_code(G, H, E, tab->sons[i], lfunc));
      affect_heap(H, addr, interp_pp_code(G, H, E, tab->sons[code->nb_sons-1], lfunc), interp_pp_code(G, H, E, code->sons[1], lfunc));
      break;
    case If:
      if(interp_pp_code(G, H, E, code->sons[0], lfunc))
	interp_pp_code(G, H, E, code->sons[1], lfunc);
      else
	interp_pp_code(G, H, E, code->sons[2], lfunc);
      break;
    case Wh:
      while(interp_pp_code(G, H, E, code->sons[0], lfunc))
	interp_pp_code(G, H, E, code->sons[1], lfunc);
      break;
    case Not:
      if(interp_pp_code(G, H, E, code->sons[0], lfunc))
	return 0;
      return 1;
      break;
    case Pl:
    case Mo:
    case Mu:
    case Or:
    case And:
    case Lt:
    case Eq:
      return operation_pp(code->def, interp_pp_code(G, H, E, code->sons[0], lfunc), interp_pp_code(G, H, E, code->sons[1], lfunc));
      break;
    case Se:
      interp_pp_code(G, H, E, code->sons[0], lfunc);
      interp_pp_code(G, H, E, code->sons[1], lfunc);
    }
}

int interp_pp_call(env G, heap H, env E, tree call, tree lfunc){
  tree f = NULL;
  env e = NULL;
  for(int i = 0; i < lfunc->nb_sons; i++){
    f = lfunc->sons[i];
    if(strcmp(call->sons[0], ((sign)f->sons[0])->name) == 0)
      break;
  }
  if(f == NULL)
    return 0;
  sign s = f->sons[0];
  if(s->type != NULL)
    e = init_env(e, s->name, s->type);
  e = concat_env(e, init_env_list(s->argt));
  for(int i = 0; i < ((tree)call->sons[1])->nb_sons; i++)
    affect_env(e, ((var)s->argt->sons[i])->name, interp_pp_code(G, H, E, ((tree)call->sons[1])->sons[i], lfunc));
  e = concat_env(e, init_env_list(f->sons[1]));
  interp_pp_code(G, H, e, f->sons[2], lfunc);
  int ret = 0;
  if(s->type != NULL)
    ret = value_env(e, s->name);
  free_env(e);
  return ret;
}

void display_tab(heap H, int add, int depth, enum define def){
  for(int i = 0; i < H->size[add]; i++){
    if(depth != 1){
      printf("[%d] = {", i);
      display_tab(H, value_heap(H, add, i), depth-1, def);
      if( i+1 < H->size[add])
	printf("}, ");
      else
	printf("}");
    }else{
      if(def == Int)
	printf("%d", value_heap(H, add, i));
      else
	printf("%s", (value_heap(H, add, i))?"true":"false");
      if( i+1 < H->size[add])
	printf(", ");
    }
  }
}

void display_env_heap(env G, heap H){
  if(G != NULL){
    display_env_heap(G->next, H);
    if(G->type->depth == 1){
      if(G->type->type[0] == Int)
	printf("var int %s = %d\n", G->name, G->value);
      else
	printf("var bool %s = %s\n", G->name, (G->value)?"true":"false");
    }else{
      printf("var array %s = {", G->name);
      display_tab(H, G->value, G->type->depth-1, G->type->type[G->type->depth - 1]);
      printf("}\n");
    }
  }
}

void free_env(env e){
  env tmp;
  while(e != NULL){
    tmp = e;
    e = e->next;
    free(tmp->name);
    free_type_exp(tmp->type);
    free(tmp);
  }
}

void free_heap(heap h){
  free(h->address);
  free(h->size);
  free(h->memory);
  free(h);
}
