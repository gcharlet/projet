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

//fonctions d'allocation mémoire pour les structures ci-dessus
extern cell alloc_cell();
extern list alloc_list();
//fonction qui permet l'espace pour une chaine de caractères
extern char* alloc_string();

//fonction qui permet de créé une instance des structures ci-dessus
extern list init_cell(char* name, enum c3a def, char* arg1, char* arg2, char* res);
//fonction qui permet de chercher un element dans l
extern cell search_cell(list l, char* name);
//fonction qui permet de concaténer deux listes
extern void concat_list(list l1, list l2);

//fonction sutilisé pour traduire le code Pseudo-pascal en C3A

//fontion qui retourne le code C3A et qui prend en parametre l'abre Pseudo-Pascal
extern list translate_pp(tree s);
//fonction qui retourne le code C3A d'une fonction et qui prend en parametre :
//la function
//trois entiers utilisé comme compteur
//la liste des fonctions
extern list translate_pp_function(tree function, int* et, int* ct, int* va, tree lfunc);
//fonction qui retourne le code C3A correspondant au code fourni, prens en parametre :
//le code
//trois entiers utilisé comme compteur
//la liste des fonctions
extern list translate_pp_code(tree code, int* et, int* ct, int* va, tree lfunc);
//fonction qui retourne le code C3A correspondant a une operation sur deux valeur, prend en parametre :
//le code
//trois entiers utilisé comme compteur
//la liste des fonctions
extern list translate_pp_operation(tree code, int* et, int* ct, int* va, tree lfunc);
//fonction qui retourne le code C3A pour un tableau
//le tableau
//la profondeur c'est a dire le nombre de fois il faudra créé une intruction IND
//trois entiers utilisé comme compteur
//la liste des fonctions
extern list translate_pp_tab(tree tab, int depth, int* et, int* ct, int* va, tree lfunc);
//fonction qui retourne le code C3A pour un appelle de fonction
//l'appelle
//trois entiers utilisé comme compteur
//la liste des fonctions
extern list translate_pp_call(tree call, int* et, int* ct, int* va, tree lfunc);
//fonction qui retourne la fonction corespondant au nom dans la liste des focntions
extern tree argt_function_pp(tree lfunc, char* name);

//fonction qui permet l'affichage d'une liste
extern void display_list(list l);

//fonction qui permet la liberation de l'espace memoire reserver
extern void free_list(list l);

#endif
