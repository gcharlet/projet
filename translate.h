#include "tree_abs.h"

#ifndef TRANSLATE_H
#define TRANSLATE_H

#define STRING_LENGTH 12

enum c3a {empty = 0, c_Pl, c_Mo, c_Mu, c_And, c_Or, c_Lt, c_Ind, c_Not, c_Af, c_Afc, c_AfInd, c_Sk, c_Jp, c_Jz, c_St, c_Param, c_Call, c_Ret};

typedef struct cell{
  char* name;
  enum c3a def;
  char* arg1;
  char* arg2;
  char* res;
  struct cell* next;
} *cell;

typedef struct list{
  cell first;
  cell end;
} *list;

// Fonctions d'allocation mémoire pour les structures ci-dessus
extern cell alloc_cell();
extern list alloc_list();
// Permet d'allouer l'espace pour une chaîne de caractères
extern char* alloc_string();

// Permet de créer une instance des structures ci-dessus
extern list init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res);
// Permet de chercher un élément dans l
extern cell search_cell(list l, char* name);
// Permet de concaténer deux listes
extern void concat_list(list l1, list l2);

// Fonctions utilisées pour traduire le code Pseudo-pascal en C3A

// Retourne le code C3A et prend en paramètre l'arbre Pseudo-Pascal
extern list translate_pp(tree s);
// Retourne le code C3A d'une fonction et prend en paramètres :
// La fonction
// Trois entiers utilisés comme compteurs
// La liste des fonctions
extern list translate_pp_function(tree function, int* et, int* ct, int* va, tree lfunc);
// Retourne le code C3A correspondant au code fourni, prend en paramètres :
// Le code
// Trois entiers utilisés comme compteurs
// La liste des fonctions
extern list translate_pp_code(tree code, int* et, int* ct, int* va, tree lfunc);
// Retourne le code C3A correspondant à une opération sur deux valeurs, prend en paramètres :
// Le code
// Trois entiers utilisés comme compteurs
// La liste des fonctions
extern list translate_pp_operation(tree code, int* et, int* ct, int* va, tree lfunc);
// Retourne le code C3A pour un tableau, prend en paramètres :
// Le tableau
// La profondeur, c'est à dire le nombre de fois où il faudra créer une instruction IND
// Trois entiers utilisés comme compteurs
// La liste des fonctions
extern list translate_pp_tab(tree tab, int depth, int* et, int* ct, int* va, tree lfunc);
// Retourne le code C3A pour un appel de fonction :
// L'appel
// Trois entiers utilisés comme compteurs
// La liste des fonctions
extern list translate_pp_call(tree call, int* et, int* ct, int* va, tree lfunc);
// Retourne la fonction corespondant au nom dans la liste des fonctions
extern tree argt_function_pp(tree lfunc, char* name);


// Permet l'affichage d'une liste
extern void display_list(list l);

// Permet la libération de l'espace mémoire réservé
extern void free_list(list l);

#endif
