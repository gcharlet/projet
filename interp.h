#include "tree_abs.h"
#include "translate.h"

#ifndef INTERP_H
#define INTERP_H

typedef struct env {
  char* name;
  type_exp type;
  int value;
  struct env* next;
} *env;

typedef struct heap {
  int* address;
  int* size;
  int* memory;
  int last_memory;
  int last_address;
  int error;
} *heap;

typedef struct pile {
  cell c;
  env l;
  struct pile* next;
} *pile;

// Fonctions d'allocation mémoire pour les structures ci-dessus
extern env alloc_env();
extern heap alloc_heap();
extern pile alloc_pile();

// Fonctions permettant de créer une instance des structures ci-dessus
extern env init_env(env e, char* name, type_exp type);
extern env init_env_list(tree s);
extern env init_env_c3a(env e, char* name);
extern env init_env_p(env p, char* name, int value);
extern heap init_heap(int space_address, int space_memory);
// Permet d'empiler c et l'environnement l puis retourne la pile
extern pile stack(cell c, env l, pile daddy);
// Permet de dépiler le premier élément de la pile puis de le retourner, et affecte à l l'environnement local de l'élement retourné
extern cell unstack(pile *p, env *l);
// Réinitialise les valeurs d'un environnement à 0
extern void reset_value(env e);

// Concatène deux environnements
extern env concat_env(env e1, env e2);
// Affecte une valeur à un élément de l'environnement
extern void affect_env(env e, char* name, int value);
// Retourne l'environnement à partir du nom passé en paramètre
extern env search_env(env e, char* name);
// Retourne la valeur d'un élément dans l'environnement
extern int value_env(env e, char* name);
// Crée un tableau dans heap et retourne son adresse
extern int new_array(heap h, int size);
// Récupère une valeur à partir de son adresse et de son indice
extern int value_heap(heap h, int address, int indice);
// Affecte une valeur à partir de son adresse et de son indice
extern int affect_heap(heap h, int address, int indice, int value);

// Fonctions utilisées pour interpréter le code Pseudo-pascal

// Interprète le code en Pseudo-Pascal et prend en paramètres l'adresse de l'environnement global G, l'adresse de la struture heap H utilisée pour le stockage des tableaux et l'arbre s
// Retourne 0 si il n'y pas eu d'erreur d'accès mémoire
extern int interp_pp(env *G, heap *H, tree s);
// Retourne la valeur d'une opération entre deux valeurs
extern int operation_pp(enum define def, int val1, int val2);
// Retourne la valeur de la variable name, soit de l'environnement local E si non nul, soit de l'environnement global G
extern int value_env_pp(env G, env E, char* name);
// Interprète le code ou une partie de code et prend en paramètres :
// Environnement global G
// Mémoire des tableaux H
// Environnement local E : non nul si on est dans une fonction, nul sinon
// Code à interpréter
// Liste des fonctions
// Retourne 0 si il n'y pas eu d'erreur d'accès mémoire
extern int interp_pp_code(env G, heap H, env E, tree code, tree lfunc);
// Interprète un appel de fonction et qui prend en paramètre :
// Environnement global G
// Mémoire des tableaux H
// Environnement local E : non nul si on est dans une fonction, nul sinon
// Code à interpréter
// Liste des fonctions
// Retourne le 0 si il n'y pas eu d'erreur d'accès mémoire
extern int interp_pp_call(env G, heap H, env E, tree call, tree lfunc);

// Fonctions utilisées pour interpréter le code C3A

// Interprète le code c3a et prend en paramètres l'adresse de l'environnement global G, l'adresse du tableau contenant les valeurs de tous les tableaux du programme et la liste contenant le code c3a
extern void interp_c3a(env *G, int** T, list list);
// Retourne le résultat d'une opération entre deux valeurs
extern int operation_c3a(enum c3a op, int val1, int val2);

// Fonctions permettant l'affichage de l'environnement souhaité
extern void display_tab_pp(heap H, int add, int depth, enum define def);
extern void display_env_heap_pp(env G, heap H);
extern void display_env_c3a(env G);
extern void display_tab_c3a(int* T, int size);

// Fonctions permettant la libération des l'espace mémoire utilisée par les structures
extern void free_env(env e);
extern void free_heap(heap h);

#endif
