%{
#include <stdio.h>
#include "our_c.tab.h"
%}
%option noyywrap
%%
"int" {return TIPi;}
"double" {return TIPd;}
"char" {return TIPc;}
"string" {return TIPs;}
"bool" {return TIPb;}
"main" {return MAIN;}
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
"=" {return ASSIGN;}
"=="|"<"|">"|"<="|">="|"||"|"&&" {return LOGICAL_OPERATOR;}
[0-9]|[1-9][0-9]+ {return NATURAL_NR;}
([0-9]|[1-9][0-9]+).[0-9][0-9]* {return REAL_NR;}
[0-1] {return BOOL;}
\'[0-9a-zA-Z_=+\[\]{};':",./<>?-]\' {return CHAR;}
\"[0-9a-zA-Z_=+\[\]{};':",./<>?-]*\" {return STRING;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
