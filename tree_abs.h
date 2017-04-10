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

//fonctions d'allocation mémoire pour les structures définies ci-dessus
extern tree tree_alloc();
extern type_exp type_exp_alloc();
extern var var_alloc();
extern sign sign_alloc();
extern val val_alloc();

//fonctions permettant de créer une instance d'une des structure définies ci-dessus
extern tree init_tree(enum define def);
extern type_exp init_type_exp(enum type_expression type);
extern var init_var(char* name, type_exp type);
extern sign init_sign(char* name, tree argt, type_exp type);
extern val init_val(enum type_value def, int value, char* name);

//fonction permetant de rajouter un type "type" à un type "tp" déja existe, fonction utilisé pour rajouter T_array
extern int add_type(type_exp tp, enum type_expression type);
//fonction permettant de rajouter un fils "sons" a l'arbre "s"
extern int add_son(tree s, void *son);

//fonctions permettant d'afficher l'arbre s
extern void display_tree(tree s);
extern void display_type_exp(type_exp t);
extern void display_var(var v);
extern void display_sign(sign s);
extern void display_val(val v);

//fonctions utilisé pour l'analyse sémantique

//fonction appellé pour faire l'analyse sémantique de l'abre s
extern int analyze(tree s);
//fonction qui affiche une erreur sémantique et ajoute 1 à la variable error
extern void error_analyze(char* s, tree exp, int* error);
//fonction qui retourne la copy du type passé en parametre
extern type_exp copy_type_exp(type_exp t);
//fonction qui retourne le type de la variable 'name' soit dans l'envrionnement locale func ou dans l'environnement globale s
extern type_exp type_def_var(tree s, char* name, tree func);
//function qui retourne 1 si t1 et t2 sont deux types identique sinon retourne 0
extern int verif_type_exp(type_exp t1, type_exp t2);
//fonction qui retourne 1 si l'appelle de la fonction correspond à une fonction définie dans le Pseudo-pascal sinon retourne -2 si la fonction n'est définie, -1 si le nombre d'argument est incorrect ou 0 si il y a un argument de type diférent que celui attendu
extern int verif_call(tree s, tree call, tree func, int* error);
//fonction qui définie le type d'un tree ou def = "Val" en fonction de son fils de structure 'val'
extern void define_type_val(tree s, tree code, tree func, int* error);
//fonction qui analyse une liste de variable afin de vérifié qu'il n'y a pas de redefinition
extern void analyze_list_vart(tree s, int* error);
//fonction qui analyse un code ou une partie qui prend en parametre
//s qui est le sommet de l'arbre afin d'avoir les variable globale et la liste des fonction
//code qui est le code évalué
//func si NULL signifie que le code évalué est le programme principale, sinon le code évalué est dans une fonction
//error est le nombre d'erreur trouvé
extern void analyze_code(tree s, tree code, tree func, int* error);
//fonction qui analyse une fonction avec 's' qui est le sommet de l'arbre afin d'avoir les variable globale et la liste des fonction et error le nombre d'erreur
extern void analyze_function(tree s, int* error);

//fonctions qui libère l'espace mémoire réservé pour les structures
extern void free_tree(tree s);
extern void free_type_exp(type_exp tp);
extern void free_var(var v);
extern void free_sign(sign s);
extern void free_val(val v);

#endif
