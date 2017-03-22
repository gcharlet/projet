#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_abs.h"

tree tree_alloc();
type_var type_var_alloc();
var var_alloc();
sign sign_alloc();
val val_alloc();
call call_alloc();
new_array new_array_alloc();

tree init_tree(enum define def);
type_var init_type_var(enum type_variable type);
var init_var(char* name, type_var type);
sign init_sign(char* name, tree argt, type_var type);
val init_val(enum type_value def, int val, char* name);
call init_call(char* name, tree args);
new_array init_new_array(tree type, tree size);

int add_type(type_var tp, enum type_variable type);
int add_son(tree pere, void *son);

void afficher(tree s);

void free_tree(tree s);
void free_type_var(type_var tp);
void free_var(var v);
void free_sign(sign s);
void free_call(call c);
void free_val(val v);
