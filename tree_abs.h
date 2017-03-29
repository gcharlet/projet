#ifndef TREE_ABS_H
#define TREE_ABS_H

enum type_expression {T_bool = 0, T_int, T_array};

typedef struct type_exp {
  int depth;
  enum type_expression *type;
} *type_exp;

enum define {Mp = 0, Pl, Mo, Mu, Or, Lt, Eq, And, Not, Call, NewAr, Tab, Se, Af, AfTab, Sk, If, Wh, Largs, Lvart, Func, Lfunc, Val};

typedef struct tree {
  enum define def;
  type_exp type;
  int nb_sons;
  void **sons;
} *tree;

typedef struct var {
  char* name;
  type_exp type;
} *var;

typedef struct sign {
  char* name;
  tree argt;
  type_exp type;
} *sign;

enum type_value {Bool = 0, Int, Var};

typedef struct val {
  enum type_value def;
  union {
    int val;
    char* name;
  } param;
} *val;

extern tree tree_alloc();
extern type_exp type_exp_alloc();
extern var var_alloc();
extern sign sign_alloc();
extern val val_alloc();

extern tree init_tree(enum define def);
extern type_exp init_type_exp(enum type_expression type);
extern var init_var(char* name, type_exp type);
extern sign init_sign(char* name, tree argt, type_exp type);
extern val init_val(enum type_value def, int value, char* name);

extern int add_type(type_exp tp, enum type_expression type);
extern int add_son(tree pere, void *son);

extern void display_tree(tree s);
extern void display_type_exp(type_exp t);
extern void display_var(var v);
extern void display_sign(sign s);
extern void display_val(val v);

extern void error_analize(char* s, tree exp, int* error);
extern type_exp type_def_var(tree s, char* name, tree func);
extern int verif_type_exp(type_exp t1, type_exp t2);
extern int verif_call(tree s, tree call, tree func, int* error);
extern void define_type_val(tree s, tree code, tree func, int* error);
extern void analize_code(tree s, tree code, tree func, int* error);
extern void analize_function(tree s, int* error);
extern int analize(tree s);

extern void free_tree(tree s);
extern void free_type_exp(type_exp tp);
extern void free_var(var v);
extern void free_sign(sign s);
extern void free_val(val v);

#endif
