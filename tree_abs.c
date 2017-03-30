#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_abs.h"

tree tree_alloc(){
  return malloc(sizeof(struct tree));
}

type_exp type_exp_alloc(){
  return malloc(sizeof(struct type_exp));
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
  s->type = NULL;
  return s;
}

type_exp init_type_exp(enum type_expression type){
  type_exp t = type_exp_alloc();
  t->depth = 1;
  t->type = malloc(sizeof(enum type_expression));
  t->type[0] = type;
  return t;
}

var init_var(char* name, type_exp type){
  var c = var_alloc();
  c->name = name;
  c->type = type;
  return c;
}

sign init_sign(char* name, tree argt, type_exp type){
  sign s = sign_alloc();
  s->name = name;
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
      v->param.name = name;
      break;
    }
  return v;
}

int add_type(type_exp tp, enum type_expression type){
  tp->depth += 1;
  tp->type = realloc(tp->type, sizeof(enum type_expression) * tp->depth);
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
  pere->nb_sons += 1;
  pere->sons = realloc(pere->sons, sizeof(void *) * pere->nb_sons);
  pere->sons[pere->nb_sons - 1] = son;
  return EXIT_SUCCESS;
}

char *list_def[] = {"Mp", "Pl", "Mo", "Mu", "Or", "Lt", "Eq", "And", "Not", "Call", "NewAr", "Tab", "Se", "Af", "Af", "Sk", "IfThEl", "Wh", "Largs", "Lvart", "Func", "List_function_procedure", "Val"};

void display_tree(tree s){
  int i = 0;
  switch (s->def)
    {
    case Mp:
      for(i; i < s->nb_sons; i++){
	printf("#######################\n");
	display_tree(s->sons[i]);
	printf("\n#######################\n\n");
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
      display_type_exp(s->sons[0]);
      i = 1;
      break;
    case Tab:
      printf("%s", (char*)s->sons[0]);
      for(i = 1; i < s->nb_sons; i++){
	printf("[ ");
	display_tree(s->sons[i]);
	printf("]");
      }
      printf(" ");
      break;
    case Val:
      display_val(s->sons[0]);
      i = 1;
      break;
    case Func:
      printf("\n\nFunc ");
      display_sign(s->sons[0]);
      printf("\n");
      display_tree(s->sons[1]);
      printf("\n");
      display_tree(s->sons[2]);
      i = 3;
      break;
    case Lvart:
      printf("List_vart { ");
      for(i; i < s->nb_sons; i++){
	display_var(s->sons[i]);
      }
      printf("} ");
      break;
    case Largs:
      printf("List_args { ");
      for(i; i < s->nb_sons; i++){
	display_tree(s->sons[i]);
      }
      printf("} ");
      break;
    default :
      printf("%s ", list_def[s->def]);
    }
  for(i; i < s->nb_sons; i++){
    display_tree(s->sons[i]);
  }
}

char *list_type[] = {"T_bool", "T_int", "T_array"};
void display_type_exp(type_exp t){
  for(int i = 0; i < t->depth; i++)
    printf("%s ", list_type[t->type[i]]);
}

void display_var(var v){
  printf("%s : ", v->name);
  display_type_exp(v->type);
}

void display_sign(sign s){
  printf("%s ", s->name);
  display_tree(s->argt);
  if(s->type != NULL){
    printf(": ");
    display_type_exp(s->type);
  }
}

void display_val(val v){
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

void error_analize(char* s, tree exp, int* error){
  printf("TYPEERROR: %s : ", s);
  display_tree(exp);
  printf("\n");
  *error += 1;
}

type_exp copy_type_exp(type_exp t){
  type_exp tmp = init_type_exp(t->type[t->depth - 1]);
  for(int i = t->depth - 2; i >= 0; i--)
    add_type(tmp, t->type[i]);
  return tmp;
}

type_exp type_def_var(tree s, char* name, tree func){
  if(s->def != Lvart)
    return NULL;
  if(func != NULL){
    if(((sign)func->sons[0])->type != NULL && strcmp(((sign)func->sons[0])->name, name) == 0)
      return ((sign)func->sons[0])->type;
    for(int i = 0; i < ((sign)func->sons[0])->argt->nb_sons; i++){
      var tmp = ((sign)func->sons[0])->argt->sons[i];
      if(strcmp(tmp->name, name) == 0)
	return tmp->type;
    }
    for(int i = 0; i < ((tree)func->sons[1])->nb_sons; i++){
      var tmp = ((tree)func->sons[1])->sons[i];
      if(strcmp(tmp->name, name) == 0)
	return tmp->type;
    }
  }
  for(int i = 0; i < s->nb_sons; i++){
    var tmp = s->sons[i];
    if(strcmp(tmp->name, name) == 0)
      return tmp->type;
  }
  return NULL;
}

int verif_type_exp(type_exp t1, type_exp t2){
  if(t1 == NULL || t2 == NULL)
    if(t1 != t2)
      return 0;
    else
      return 1;
  if(t1->depth != t2->depth)
    return 0;
  for(int i = 0; i < t1->depth; i++)
    if(t1->type[i] != t2->type[i])
      return 0;
  return 1;
}

int verif_call(tree s, tree call, tree func, int* error){
  tree f = NULL;
  tree lfunc = s->sons[1];
  for(int i = 0; i < lfunc->nb_sons; i++){
    if(strcmp(call->sons[0], ((sign)((tree)lfunc->sons[i])->sons[0])->name) == 0)
      f = lfunc->sons[i];
  }
  if(f == NULL)
    return -2;
  if(((sign)f->sons[0])->type != NULL)
    call->type = copy_type_exp(((sign)f->sons[0])->type);
  tree argt = ((sign)f->sons[0])->argt;
  if(argt->nb_sons != ((tree)call->sons[1])->nb_sons)
    return -1;
  for(int i = 0; i < argt->nb_sons; i++){
    analize_code(s, ((tree)call->sons[1])->sons[i], func, error);
    if(verif_type_exp(((var)argt->sons[i])->type, ((tree)((tree)call->sons[1])->sons[i])->type) == 0)
      return 0;
  }
  return 1;
}

void define_type_val(tree s, tree code, tree func, int* error){
  switch (((val)code->sons[0])->def)
  {
  case Bool:
    code->type = init_type_exp(T_bool);
    break;
  case Int:
    code->type = init_type_exp(T_int);
    break;
  case Var:
    code->type = copy_type_exp(type_def_var(s->sons[0], ((val)code->sons[0])->param.name, func));
    if(code->type == NULL){
      printf("VARERROR: variable non définie : %s\n", ((val)code->sons[0])->param.name);
      *error += 1;
    }
    break;
  }
}

void analize_code(tree s, tree code, tree func, int* error){
  switch (code->def)
    {
    case Val:
      define_type_val(s, code, func, error);
      return;
      break;
    case Call:
      ;
      int j = verif_call(s, code, func, error);
      if(j == -2){
	printf("CALLERROR: fonction non définie : %s\n", (char*)code->sons[0]);
	*error += 1;
      }
      if(j == -1){
	printf("CALLERROR: nombre de parametre incorrect : %s\n", (char*)code->sons[0]);
	*error += 1;
      }
      if(j == 0)
	error_analize("type args diférents de la définition de la fonction", code, error);
      return;
      break;
    case NewAr:
      analize_code(s, code->sons[1], func, error);
      type_exp ar = code->sons[0];
      int i = ar->depth - 1;
      code->type = init_type_exp(ar->type[i]);
      for(i -= 1; i >= 0; i--)
	add_type(code->type, ar->type[i]);
      add_type(code->type, T_array);
      if(((tree)code->sons[1])->type == NULL || ((tree)code->sons[1])->type->type[0] != T_int)
	error_analize("taille de type non-entier", code, error);
      return;
      break;
    case Tab:
      ;
      type_exp tab = type_def_var(s->sons[0], code->sons[0], func);
      int nb = tab->depth;
      if(tab == NULL){
        printf("VARERROR: tableau non définie : %s\n", (char*)code->sons[0]);
	*error += 1;
      }
      for(int i = 1; i < code->nb_sons; i++){
	analize_code(s, code->sons[i], func, error);
	nb--;
	if(((tree)code->sons[i])->type == NULL || ((tree)code->sons[i])->type->type[0] != T_int)
	  error_analize("index non-entier dans un tableau", code->sons[i], error);
      }
      if(nb <= 0){
	error_analize("trop d'index par rapport à la définition du tableau", code, error);
	code->type = NULL;
      }else{
	code->type = init_type_exp(tab->type[tab->depth - 1]);
	for(int i = 1; i < nb; i++)
	  add_type(code->type, tab->type[tab->depth - i - 1]);
      }
      return;
      break;
    case Af:
      analize_code(s, code->sons[1], func, error);
      type_exp t = type_def_var(s->sons[0], code->sons[0], func);
      if(t == NULL || verif_type_exp(t, ((tree)code->sons[1])->type) == 0)
	error_analize("affectation de type incoherents", code, error);
      return;
      break;
    }
  
  for(int i = 0; i < code->nb_sons; i++)
    analize_code(s, code->sons[i], func, error);
  
  switch (code->def)
    {
    case Pl:
    case Mu:
    case Mo:
      code->type = init_type_exp(T_int);
      if(verif_type_exp(((tree)code->sons[0])->type, code->type) == 0 || verif_type_exp(((tree)code->sons[1])->type, code->type) == 0)
	error_analize("op entier sur args non-entiers", code, error);
      break;
    case Or:
    case And:
      code->type = init_type_exp(T_bool);
      if(verif_type_exp(((tree)code->sons[0])->type, code->type) == 0 || verif_type_exp(((tree)code->sons[1])->type, code->type) == 0)
	error_analize("op boolean sur args non-boolean", code, error);
      break;
    case Not:
      code->type = init_type_exp(T_bool);
      if(verif_type_exp(((tree)code->sons[0])->type, code->type) == 0)
	error_analize("op boolean sur args non-boolean", code, error);
      break;
    case Lt:
      code->type = init_type_exp(T_bool);
      if(((tree)code->sons[0])->type == NULL || ((tree)code->sons[0])->type->type[0] != T_int || verif_type_exp(((tree)code->sons[0])->type, ((tree)code->sons[1])->type) == 0)
	error_analize("op boolean sur args non-entier", code, error);
      break;
    case Eq:
      code->type = init_type_exp(T_bool);
      if(((tree)code->sons[1])->type == NULL || verif_type_exp(((tree)code->sons[0])->type, ((tree)code->sons[1])->type) == 0)
	error_analize("op boolean sur args de differents type", code, error);
      break;
    case AfTab:
      if(((tree)code->sons[0])->type == NULL || verif_type_exp(((tree)code->sons[0])->type, ((tree)code->sons[1])->type) == 0)
	error_analize("affectation de type incoherents", code, error);
      break;
    case If:
    case Wh:
      if(((tree)code->sons[0])->type == NULL || ((tree)code->sons[0])->type->type[0] != T_bool)
	error_analize("condition de type boolean attendue", code, error);
      break;
    }
}

void analize_list_vart(tree s, int* error){
  if(s->def != Lvart)
    return;
  for(int i = 0; i < s->nb_sons; i++){
    for(int j = i + 1; j < s->nb_sons; j++){
      if(strcmp(((var)s->sons[i])->name, ((var)s->sons[j])->name) == 0){
	printf("VARERROR : variable déja définie dans la liste : %s\n", ((var)s->sons[j])->name);
	*error += 1;
      }
    }
  }
}

void analize_function(tree s, int* error){
  tree lfunc = s->sons[1];
  for(int i = 0; i < lfunc->nb_sons; i++){
    analize_list_vart(((sign)((tree)lfunc->sons[i])->sons[0])->argt, error);
    analize_list_vart(((tree)lfunc->sons[i])->sons[1], error);
    analize_code(s, ((tree)lfunc->sons[i])->sons[2], lfunc->sons[i], error);
  }
}

int analize(tree s){
  int error = 0;
  analize_list_vart(s->sons[0], &error);
  analize_function(s, &error);
  analize_code(s, s->sons[2], NULL, &error);
  if(error == 0)
    printf("Analyse sémantique validé\n\n");
  return error;
}

void free_tree(tree s);
void free_type_exp(type_exp tp);
void free_var(var v);
void free_sign(sign s);
void free_val(val v);
