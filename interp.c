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
  for(int i = 0; i < space_address; i++){
    h->address[i] = 0;
    h->size[i] = 0;
  }
  for(int i = 0; i < space_memory; i++)
    h->memory[i] = 0;
  return h;
}

env concat_env(env e1, env e2){
  env e = e2;
  while(e2->next != NULL)
    e2 = e2->next;
  e2->next = e1;
  return e2;
}

void affect_env(env e, char* name, int value){
  env p = search_env(e, name);
  if(p != NULL)
    p->value = value;
}

env search_env(env e, char* name){
  if(e == NULL)
    return NULL;
  env p = e;
  while(strcmp(p->name, name) != 0 && p->next != NULL)
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
  if(i == 0 || indice >= h->size[address])
    return 0;
  return h->memory[i+indice];
}

int affect_heap(heap h, int address, int indice, int value){
  if(h->address[address] == 0 || indice >= h->size[address])
    return -1;
  h->memory[h->address[address] + indice] = value;
  return 0;
}

void free_env();
void free_heap();
