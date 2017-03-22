#ifndef TREE_ABS_H
#define TREE_ABS_H

enum define {Mp = 0, Pl, Mo, Mu, Or, Lt, Eq, And, Not, Call, NewAr, Tab, Se, Af, Sk, If, Wh, Largs, Lvart, Type, Func, Lfunc};

typedef struct tree {
  enum define def;
  int nb_sons;
  void **sons;
} *tree;

enum type_var {T_bool = 0, T_int, T_array};

typedef struct type_var {
  int depth;
  enum type_var *type;
} *type_var;

typedef struct var {
  char* name;
  type_var type;
} *var;

typedef struct sign {
  char* name;
  tree argt;
  type_var type;
} *sign;

enum type_value {Int, Var, Bool};

typedef struct val {
  enum type_value def;
  union {
    int val;
    char* name;
  } param;
} *val;

typedef struct call {
  char* name;
  tree args;
} *call;

typedef struct new_array {
  tree type;
  tree size;
} *new_array;

extern tree tree_alloc();
extern type_var type_var_alloc();
extern var var_alloc();
extern sign sign_alloc();
extern val val_alloc();
extern call call_alloc();
extern new_array new_array_alloc();

extern tree init_tree(enum define def);
extern type_var init_type_var(enum type_var type);
extern var init_var(char* name, type_var type);
extern sign init_sign(char* name, tree argt, type_var type);
extern val init_val(enum type_value def, int val, char* name);
extern call init_call(char* name, tree args);
extern new_array init_new_array(tree type, tree size);

extern int add_type(type_var tp, enum type_var type);
extern int add_son(tree pere, void son);

extern void afficher(tree s);

extern void free_tree(tree s);
extern void free_type_var(type_var tp);
extern void free_var(var v);
extern void free_sign(sign s);
extern void free_call(call c);
extern void free_val(val v);

#endif
