%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppascal.tab.h"
%}

Nombre 0|[1-9][0-9]*
Nom [A-Za-z][A-Za-z0-9\_]*
%s COMM

%%
<INITIAL>"/*" {BEGIN COMM;}
<COMM>.|\n {}
<COMM>"*/" {BEGIN INITIAL;}
<INITIAL>"boolean" {return(BOO);}
<INITIAL>"integer" {return(INT);}
<INITIAL>"array of" {return(AR);}
<INITIAL>"new array of" {return(NEWAR);}
<INITIAL>"defun" {return(FUNC);}
<INITIAL>"defpro" {return(PROC);}
<INITIAL>"var" {return(VAR);}
<INITIAL>":=" {return(AF);}
<INITIAL>"+" {return(PL);}
<INITIAL>"-" {return(MO);}
<INITIAL>"*" {return(MU);}
<INITIAL>"<" {return(LT);}
<INITIAL>"=" {return(EQ);}
<INITIAL>"and" {return(AND);}
<INITIAL>"or" {return(OR);}
<INITIAL>"not" {return(NOT);}
<INITIAL>"while" {return(WH);}
<INITIAL>"do" {return(DO);}
<INITIAL>"if" {return(IF);}
<INITIAL>"then" {return(TH);}
<INITIAL>"else" {return(EL);}
<INITIAL>"skip" {return(SK);}
<INITIAL>";" {return(SE);}
<INITIAL>"{" {return('{');}
<INITIAL>"}" {return('}');}
<INITIAL>"(" {return('(');}
<INITIAL>")" {return(')');}
<INITIAL>"[" {return('[');}
<INITIAL>"]" {return(']');}
<INITIAL>":" {return(':');}
<INITIAL>"," {return(',');}
<INITIAL>"true" {return(TRUE);}
<INITIAL>"false" {return(FALSE);}
<INITIAL>{Nombre} {yylval.val = atoi(yytext); return(I);}
<INITIAL>{Nom} {yylval.str = malloc(sizeof(char) * yyleng+1); strcpy(yylval.str, yytext); return(V);}
##<INITIAL>{Nom} {yylval.str = strdup(yytext); return(V);}
[ \n\t] {}
%%
