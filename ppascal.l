%{
#include "ppascal.tab.h"
#include <string.h>
%}

Nombre 0|[1-9][0-9]*
Nom [A-Za-z][A-Za-z0-9\_]*

%%
"boolean" {return(BOO);}
"integer" {return(INT);}
"array of" {return(AR);}
"new array of" {return(NEWAR);}
"defun" {return(FUNC);}
"depro" {return(PROC);}
"var" {return(VAR);}
":=" {return(AF);}
"+" {return(PL);}
"-" {return(MO);}
"*" {return(MU);}
"<=" {return(LT);}
"=" {return(EQ);}
"and" {return(AND);}
"or" {return(OR);}
"not" {return(NOT);}
"while" {return(WH);}
"do" {return(DO);}
"if" {return(IF);}
"then" {return(TH);}
"else" {return(EL);}
"skip" {return(SK);}
";" {return(SE);}
"(" {return('(');}
")" {return(')');}
"[" {return('[');}
"]" {return(']');}
":" {return(':');}
"," {return(',');}
"true" {return(TRUE);}
"false" {return(FALSE);}
{Nombre} {yylval.val = atoi(yytext); return(I);}
{Nom} {yylval.str = malloc(yyleng * sizeof(char)); strcpy(yylval.str, yytext); return(V);}
[ \n\t] {}
. {}
%%
