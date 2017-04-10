/* CHARLET Guillaume et GAUTIER Florian
 * Question 1 :
 * Fichier tree_abs.c, fonction yyparse();
 * Question 2 :
 * Fichier tree_abs.c, fonction analyze(tree s);
 * Question 3 :
 * Fichier interp.c, fonction interp_pp(env *G, heap *H, tree s);
 * Question 4 :
 * Fichier translate.c, fonction translate_pp(tree s);
 * Question 5 :
 * Fichier interp.c, fonction interp_c3a(env *G, int** T, list list);
 */

%{
  #define _XOPEN_SOURCE 500
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <fcntl.h>
  #include <unistd.h>
  #include "tree_abs.h"
  #include "interp.h"
  #include "translate.h"
  
  int yyerror(char *s);
  int yylex();
  tree s;
%}

%union {
  char *str;
  int   val;
  void *s;
}

%start MP
%token<str> V
%token<val> I
%token BOO INT AR NEWAR FUNC PROC VAR AF WH DO IF TH EL SK SE TRUE FALSE
%left OR AND LT EQ
%left PL MO
%left MU
%left NOT
%type<s> E C0 C Et L_args L_argsnn L_argt L_argtnn Argt TP L_vart L_vartnn D_entp D_entf D LD MP

%%
MP: L_vart LD C {$$ = init_tree(Mp); add_son($$, $1); add_son($$, $2); add_son($$, $3); s = $$;}
;

E: E PL E {$$ = init_tree(Pl); add_son($$, $1); add_son($$, $3);}
| E MO E {$$ = init_tree(Mo); add_son($$, $1); add_son($$, $3);}
| E OR E {$$ = init_tree(Or); add_son($$, $1); add_son($$, $3);}
| E LT E {$$ = init_tree(Lt); add_son($$, $1); add_son($$, $3);}
| E EQ E {$$ = init_tree(Eq); add_son($$, $1); add_son($$, $3);}
| E AND E {$$ = init_tree(And); add_son($$, $1); add_son($$, $3);}
| E MU E {$$ = init_tree(Mu); add_son($$, $1); add_son($$, $3);}
| NOT E {$$ = init_tree(Not); add_son($$, $2);}
| '(' E ')' {$$ = $2;}
| I {$$ = init_tree(Val); add_son($$, init_val(Int, $1, NULL));}
| V {$$ = init_tree(Val); add_son($$, init_val(Var, 0, $1));}
| TRUE {$$ = init_tree(Val); add_son($$, init_val(Bool, 1, NULL));}
| FALSE {$$ = init_tree(Val); add_son($$, init_val(Bool, 0, NULL));}
| Et
| V '(' L_args ')' {$$ = init_tree(Call); add_son($$, $1); add_son($$, $3);}
| NEWAR TP '[' E ']' {$$ = init_tree(NewAr); add_son($$, $2); add_son($$, $4);}
;

Et: V '[' E ']' {$$ = init_tree(Tab); add_son($$, $1); add_son($$, $3);}
| Et '[' E ']' {add_son($1, $3); $$ = $1;}
;

C: C SE C0 {$$ = init_tree(Se); add_son($$, $1); add_son($$, $3);}
| C0
;


C0 : Et AF E {$$ = init_tree(AfTab); add_son($$, $1); add_son($$, $3);}
| V AF E  {$$ = init_tree(Af); add_son($$, $1); add_son($$, $3);}
| SK {$$ = init_tree(Sk);}
| '{' C '}' {$$ = $2;}
| IF E TH C0 EL C0 {$$ = init_tree(If); add_son($$, $2); add_son($$, $4); add_son($$, $6);}
| WH E DO C0 {$$ = init_tree(Wh); add_son($$, $2); add_son($$, $4);}
| V '(' L_args ')' {$$ = init_tree(Call); add_son($$, $1); add_son($$, $3);}
;

L_args: %empty {$$ = init_tree(Largs);}
| L_argsnn
;

L_argsnn: E {$$ = init_tree(Largs); add_son($$, $1);}
| L_argsnn ',' E {add_son($1, $3); $$ = $1;}
;

L_argt: %empty {$$ = init_tree(Lvart);}
| L_argtnn
;

L_argtnn: Argt {$$ = init_tree(Lvart); add_son($$, $1);}
| L_argtnn ',' Argt {add_son($1, $3); $$ = $1;}
;

Argt: V ':' TP {$$ = init_var($1, $3);}
;

TP: BOO {$$ = init_type_exp(T_bool);}
| INT {$$ = init_type_exp(T_int);}
| AR TP {add_type($2, T_array); $$ = $2;}
;

L_vart: %empty {$$ = init_tree(Lvart);}
| L_vartnn
;

L_vartnn: VAR Argt {$$ = init_tree(Lvart); add_son($$, $2);}
| L_vartnn ',' VAR Argt {add_son($1, $4); $$ = $1;}
;

D_entp: PROC V '(' L_argt ')' {$$ = init_sign($2, $4, NULL);}
;

D_entf: FUNC V '(' L_argt ')' ':' TP {$$ = init_sign($2, $4, $7);}
;

D: D_entp L_vart C {$$ = init_tree(Func); add_son($$, $1); add_son($$, $2); add_son($$, $3);}
| D_entf L_vart C {$$ = init_tree(Func); add_son($$, $1); add_son($$, $2); add_son($$, $3);}
;

LD: %empty {$$ = init_tree(Lfunc);}
| LD D {add_son($1, $2); $$ = $1;}
;
%%

int yyerror(char *str){
  fprintf(stderr, "***ERROR:%s***\n", str);
  s = NULL;
  return -1;
}

void main(){
  yyparse();
  if(s == NULL)
    return;
  //affichage de l'analyse syntaxique
  display_tree(s);

  //analyse sémantique
  int error = analyze(s);
  if(error != 0){
    free_tree(s);
    return;
  }

  env G = NULL;
  heap H = NULL;
  
  //interpretation du code Pseudo-pascal et affichage de l'environnement gloabale
  error = interp_pp(&G, &H, s);
  printf("Variables d'environnement globale du Pseudo-Pascal\n");
  display_env_heap_pp(G, H);
  printf("Fin d'environnement globale du Pseudo-Pascal\n");
  if(H != NULL)
    free_heap(H);
  if(error != 0){
    if(G != NULL)
      free_env(G);
    return;
  }

  //traduction du code en C3A
  printf("\n");
  list l = translate_pp(s);
  free_tree(s);

  //ecriture du code C3A dans le fichier TRANSLATE_C3A.c3a
  int fd = open("TRANSLATE_C3A.c3a", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  int terminal = dup(1);
  dup2(fd, 1);
  display_list(l);
  dup2(terminal, 1);

  //interpretation du code C3A et affichage de l'environmment globale ainsi que des valeurs stocké pour les tableaux
  int* T = NULL;
  reset_value(G);
  interp_c3a(&G, &T, l);
  printf("Variables d'environnement globale du C3A\n");
  display_env_c3a(G);
  display_tab_c3a(T, value_env(G, "L_TAB#"));
  printf("Fin d'environnement globale du C3A\n");

  free(T);
  free_env(G);
  
  free_list(l);

  
}

