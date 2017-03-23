#ifndef TREE_ABS_H
#define TREE_ABS_H

enum define {Mp = 0, Pl, Mo, Mu, Or, Lt, Eq, And, Not, Call, NewAr, Tab, Se, Af, AfTab, Sk, If, Wh, Largs, Lvart, Func, Lfunc, Val};

typedef struct tree {
  enum define def;
  int nb_sons;
  void **sons;
} *tree;

enum type_variable {T_bool = 0, T_int, T_array};

typedef struct type_var {
  int depth;
  enum type_variable *type;
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

extern tree tree_alloc();
extern type_var type_var_alloc();
extern var var_alloc();
extern sign sign_alloc();
extern val val_alloc();

extern tree init_tree(enum define def);
extern type_var init_type_var(enum type_variable type);
extern var init_var(char* name, type_var type);
extern sign init_sign(char* name, tree argt, type_var type);
extern val init_val(enum type_value def, int value, char* name);

extern int add_type(type_var tp, enum type_variable type);
extern int add_son(tree pere, void *son);

extern void afficher(tree s);
extern void afficher_type_var(type_var t);
extern void afficher_var(var v);
extern void afficher_sign(sign s);
extern void afficher_val(val v);

extern void free_tree(tree s);
extern void free_type_var(type_var tp);
extern void free_var(var v);
extern void free_sign(sign s);
extern void free_val(val v);

#endif
