#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_abs.h"

tree tree_alloc(){
  return malloc(sizeof(struct tree));
}

type_var type_var_alloc(){
  return malloc(sizeof(struct type_var));
}

var var_alloc(){
  return malloc(sizeof(struct var));
}

sign sign_alloc(){
  return malloc(sizeof(struct sign));
}

val val_alloc(){
  return malloc(sizeof(struct val));
}


tree init_tree(enum define def){
  tree s = tree_alloc();
  s->def = def;
  s->nb_sons = 0;
  s->sons = NULL;
  return s;
}

type_var init_type_var(enum type_variable type){
  type_var t = type_var_alloc();
  t->depth = 1;
  t->type = malloc(sizeof(enum type_variable));
  t->type[0] = type;
  return t;
}

var init_var(char* name, type_var type){
  var c = var_alloc();
  c->name = strdup(name);
  c->type = type;
  return c;
}

sign init_sign(char* name, tree argt, type_var type){
  sign s = sign_alloc();
  s->name = strdup(name);
  s->argt = argt;
  s->type = type;
  return s;
}

val init_val(enum type_value def, int value, char* name){
  val v = val_alloc();
  v->def = def;
  switch (def)
    {
    case Bool :
    case Int :
      v->param.val = value;
      break;
    case Var :
      v->param.name = strdup(name);
      break;
    }
  return v;
}

int add_type(type_var tp, enum type_variable type){
  tp->depth += 1;
  tp->type = realloc(tp->type, sizeof(enum type_variable) * tp->depth);
  for(int i = tp->depth-1; i > 0; i--)
    tp->type[i] = tp->type[i-1];
  tp->type[0] = type;
  return EXIT_SUCCESS;
}

int add_son(tree pere, void *son){
  switch (pere->def)
    {
    case Sk:
      return EXIT_FAILURE;
    case Not:
    case Val:
      if(pere->nb_sons == 1)
	return EXIT_FAILURE;
      break;
    case Call:
    case Af:
    case AfTab:
    case NewAr:
    case Pl:
    case Mo:
    case Mu:
    case Or:
    case Lt:
    case Eq:
    case And:
    case Se:
    case Wh:
      if(pere->nb_sons == 2)
	return EXIT_FAILURE;
      break;
    case Mp:
    case If:
    case Func:
      if(pere->nb_sons == 3)
	return EXIT_FAILURE;
      break;
    }
  if(pere->nb_sons == 0 && (pere->def == Af || pere->def == Call || pere->def == Tab))
    son = strdup(son);
  pere->nb_sons += 1;
  pere->sons = realloc(pere->sons, sizeof(void *) * pere->nb_sons);
  pere->sons[pere->nb_sons - 1] = son;
  return EXIT_SUCCESS;
}

char *list_def[] = {"Mp", "Pl", "Mo", "Mu", "Or", "Lt", "Eq", "And", "Not", "Call", "NewAr", "Tab", "Se", "Af", "Af", "Sk", "IfThEl", "Wh", "Largs", "Lvart", "Func", "List_function_procedure", "Val"};

void afficher(tree s){
  int i = 0;
  switch (s->def)
    {
    case Mp:
      printf("MP\n");
      for(i; i < s->nb_sons; i++){
	afficher(s->sons[i]);
	printf("\n");
      }
      break;
    case Af:
      printf("%s %s ", list_def[s->def], (char*)s->sons[0]);
      i = 1;
      break;
    case Call:
      printf("%s ", (char*)s->sons[0]);
      i = 1;
      break;
    case NewAr:
      printf("%s ", list_def[s->def]);
      afficher_type_var(s->sons[0]);
      i = 1;
      break;
    case Tab:
      printf("%s", (char*)s->sons[0]);
      for(i = 1; i < s->nb_sons; i++){
	printf("[ ");
	afficher(s->sons[i]);
	printf("]");
      }
      printf(" ");
      break;
    case Val:
      afficher_val(s->sons[0]);
      i = 1;
      break;
    case Func:
      printf("\nFunc ");
      afficher_sign(s->sons[0]);
      printf("\n");
      afficher(s->sons[1]);
      printf("\n");
      afficher(s->sons[2]);
      i = 3;
      break;
    case Lvart:
      printf("List_vart { ");
      for(i; i < s->nb_sons; i++){
	afficher_var(s->sons[i]);
      }
      printf("} ");
      break;
    case Largs:
      printf("List_args { ");
      for(i; i < s->nb_sons; i++){
	afficher(s->sons[i]);
      }
      printf("} ");
      break;
    default :
      printf("%s ", list_def[s->def]);
    }
  for(i; i < s->nb_sons; i++){
    afficher(s->sons[i]);
  }
}

char *list_type[] = {"T_bool", "T_int", "T_array"};
void afficher_type_var(type_var t){
  for(int i = 0; i < t->depth; i++)
    printf("%s ", list_type[t->type[i]]);
}

void afficher_var(var v){
  printf("%s : ", v->name);
  afficher_type_var(v->type);
}

void afficher_sign(sign s){
  printf("%s ", s->name);
  afficher(s->argt);
  printf(": ");
  if(s->type != NULL)
    afficher_type_var(s->type);
}

void afficher_val(val v){
  switch (v->def)
    {
    case Int:
      printf("%d ", v->param.val);
      break;
    case Var:
      printf("%s ", v->param.name);
      break;
    case Bool:
      if(v->param.val == 0)
	printf("false ");
      else
	printf("true ");
      break;
    }
}

void free_tree(tree s);
void free_type_var(type_var tp);
void free_var(var v);
void free_sign(sign s);
void free_val(val v);
