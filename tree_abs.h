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

// Fonctions d'allocation mémoire pour les structures définies ci-dessus
extern tree tree_alloc();
extern type_exp type_exp_alloc();
extern var var_alloc();
extern sign sign_alloc();
extern val val_alloc();

// Fonctions permettant de créer une instance d'une des structures définies ci-dessus
extern tree init_tree(enum define def);
extern type_exp init_type_exp(enum type_expression type);
extern var init_var(char* name, type_exp type);
extern sign init_sign(char* name, tree argt, type_exp type);
extern val init_val(enum type_value def, int value, char* name);

// Permet de rajouter un type "type" à un type "tp" déja existant, fonction utilisée pour rajouter T_array
extern int add_type(type_exp tp, enum type_expression type);
// Permet de rajouter un fils "son" à l'arbre "s"
extern int add_son(tree s, void *son);

// Permet d'afficher l'arbre s
extern void display_tree(tree s);
extern void display_type_exp(type_exp t);
extern void display_var(var v);
extern void display_sign(sign s);
extern void display_val(val v);

// Fonctions utilisées pour l'analyse sémantique

// Fait l'analyse sémantique de l'arbre s
extern int analyze(tree s);
// Affiche une erreur sémantique et ajoute 1 à la variable error
extern void error_analyze(char* s, tree exp, int* error);
// Retourne la copie du type passé en paramètre
extern type_exp copy_type_exp(type_exp t);
// Retourne le type de la variable 'name', soit dans l'environnement local func, soit dans l'environnement global s
extern type_exp type_def_var(tree s, char* name, tree func);
// Retourne 1 si t1 et t2 sont deux types identiques, sinon retourne 0
extern int verif_type_exp(type_exp t1, type_exp t2);
// Retourne 1 si l'appel de la fonction correspond à une fonction définie dans le Pseudo-pascal, retourne -2 si la fonction n'est pas définie, -1 si le nombre d'arguments est incorrect et 0 si il y a un argument de type différent que celui attendu
extern int verif_call(tree s, tree call, tree func, int* error);
// Définit le type d'un tree où def = "Val" en fonction de son fils de structure 'val'
extern void define_type_val(tree s, tree code, tree func, int* error);
// Analyse une liste de variables afin de vérifier qu'il n'y a pas de redéfinition
extern void analyze_list_vart(tree s, int* error);
// Analyse un code ou une partie qui prend en paramètre :
// s qui est le sommet de l'arbre afin d'avoir les variables globales et la liste des fonctions
// code qui est le code évalué
// func si NULL signifie que le code évalué est le programme principal, sinon le code évalué est dans une fonction
// error est le nombre d'erreurs trouvées
extern void analyze_code(tree s, tree code, tree func, int* error);
// Analyse une fonction avec 's' qui est le sommet de l'arbre afin d'avoir les variables globales et la liste des fonctions et error le nombre d'erreur
extern void analyze_function(tree s, int* error);

// Libère l'espace mémoire réservé pour les structures
extern void free_tree(tree s);
extern void free_type_exp(type_exp tp);
extern void free_var(var v);
extern void free_sign(sign s);
extern void free_val(val v);

#endif
