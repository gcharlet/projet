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

//fonctions d'allocation mémoire pour les structures ci-dessus
extern env alloc_env();
extern heap alloc_heap();
extern pile alloc_pile();

//fonctions permettant de créer une instance des structures ci-dessus
extern env init_env(env e, char* name, type_exp type);
extern env init_env_list(tree s);
extern env init_env_c3a(env e, char* name);
extern env init_env_p(env p, char* name, int value);
extern heap init_heap(int space_address, int space_memory);
//fonction qui permet d'empiler c et la liste l puis retourne la pile
extern pile stack(cell c, env l, pile daddy);
//fonction qui permet de dépiler le premier élément de la pile puis de le retourner, et affecte à l l'environnement local de l'élement retourné
extern cell unstack(pile *p, env *l);
//fonction qui remet les valeurs d'un environnement à 0
extern void reset_value(env e);

//fonction qui permet de concatener deux environnement
extern env concat_env(env e1, env e2);
//fonction qui permet d'affecter une valeur à un élément de l'environement
extern void affect_env(env e, char* name, int value);
//fonction qui retourne l'environement a partir du nom passé en parametre
extern env search_env(env e, char* name);
//fonction qui retourne la valeur d'un élément dans l'environement
extern int value_env(env e, char* name);
//fonction qui permet de créér un tableaux dans heap et qui retourne son adresse
extern int new_array(heap h, int size);
//fonction qui permet de récupére une valeur a partir de son adresse et de son indice
extern int value_heap(heap h, int address, int indice);
//fonction qui affecte une valeur a partir de son adresse et de son indice
extern int affect_heap(heap h, int address, int indice, int value);

//fonctions utilisé pour interprété le code Pseudo-pascal

//fonction appellé afin d'interprété le code en Pseudo-Pascal qui prend en parametre l'adress de l'environement globale G, l'adresse de la struture heap H utilisé pour le stockage des tableaux, et l'arbre s
//retourne le 0 si il n'y pas eu d'erreur d'acces memoire
extern int interp_pp(env *G, heap *H, tree s);
//fonction qui retourne la valeur d'une opération sur deux valeurs
extern int operation_pp(enum define def, int val1, int val2);
//fonction qui retourne la valeur de la variable name soit de l'environement locale E si non null ou de l'environement globale G
extern int value_env_pp(env G, env E, char* name);
//fonction qui interprète le code ou une partie de code et qui prend en paramatre :
//environement globale G
//memoire des tableaux H
//environement locale E : non null si on est dans une fonction, null sinon
//code à interpréter
//liste des fonctions
//retourne le 0 si il n'y pas eu d'erreur d'acces memoire
extern int interp_pp_code(env G, heap H, env E, tree code, tree lfunc);
//fonction qui interprète un appelle de fonction et qui prend en paramatre :
//environement globale G
//memoire des tableaux H
//environement locale E : non null si on est dans une fonction, null sinon
//code à interpréter
//liste des fonctions
//retourne le 0 si il n'y pas eu d'erreur d'acces memoire
extern int interp_pp_call(env G, heap H, env E, tree call, tree lfunc);

//fonctions utilisé pour interprété le code C3A

extern void interp_c3a(env *G, int** T, list list);
extern int operation_c3a(enum c3a op, int val1, int val2);

//fonctions permetant l'affichage de l'environement souhaité
extern void display_tab_pp(heap H, int add, int depth, enum define def);
extern void display_env_heap_pp(env G, heap H);
extern void display_env_c3a(env G);
extern void display_tab_c3a(int* T, int size);

//fonction permettant la libération des l'espace mémoire utilisé par les structures
extern void free_env(env e);
extern void free_heap(heap h);

#endif
