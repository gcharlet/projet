%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "tree_abs.h"
  
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
MP: L_vart LD C
;

E: E PL E {$$ = NULL;}
| E MO E {$$ = NULL;}
| E OR E {$$ = NULL;}
| E LT E {$$ = NULL;}
| E EQ E {$$ = NULL;}
| E AND E {$$ = NULL;}
| E MU E {$$ = NULL;}
| NOT E {$$ = NULL;}
| '(' E ')' {$$ = NULL;}
| I {$$ = NULL;}
| V {$$ = NULL;}
| TRUE {$$ = NULL;}
| FALSE {$$ = NULL;}
| Et
| V '(' L_args ')' {$$ = NULL;}
| NEWAR TP '[' E ']' {$$ = NULL;}
;

Et: V '[' E ']' {$$ = NULL;}
| Et '[' E ']'
;

C: C SE C0
| C0;


C0 : Et AF E
| V AF E  {$$ = NULL;}
| SK {$$ = NULL;}
| '{' C '}' {$$ = NULL;}
| IF E TH C0 EL C0 {$$ = NULL;}
| WH E DO C0 {$$ = NULL;}
| V '(' L_args ')' {$$ = NULL;}
;

L_args: %empty {$$ = NULL;}
| L_argsnn
;

L_argsnn: E
| E ',' L_argsnn
;

L_argt: %empty {$$ = NULL;}
| L_argtnn
;

L_argtnn: Argt
| L_argtnn ',' Argt
;

Argt: V ':' TP {$$ = NULL;}
;

TP: BOO {$$ = NULL;}
| INT {$$ = NULL;}
| AR TP {$$ = NULL;}
;

L_vart: %empty {$$ = NULL;}
| L_vartnn
;

L_vartnn: VAR Argt {$$ = NULL;}
| L_vartnn ',' VAR Argt
;

D_entp: PROC V '(' L_argt ')' {$$ = NULL;}
;

D_entf: FUNC V '(' L_argt ')' ':' TP {$$ = NULL;}
;

D: D_entp L_vart C
| D_entf L_vart C
;

LD: %empty {$$ = NULL;}
| LD D
;
%%

int yyerror(char *s){
  fprintf(stderr, "***ERROR:%s***\n", s);
  return -1;
}

void main(){
  yyparse();
}
