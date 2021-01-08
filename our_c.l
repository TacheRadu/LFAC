%{
#include <stdio.h>
#include "our_c.tab.h"
%}
%option noyywrap
%%
"//".* ;
"int"|"char"|"string"|"bool"|"float"|"class" {return TIP;}
"const" {return CONST;}
"main" {return MAIN;}
"true"|"false" {return BOOL;}
"=" {return ASSIGN;}
"=="|"<"|">"|"<="|">="|"||"|"&&" {return LOGICAL_OPERATOR;}
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
[0-9]|[1-9][0-9]+ {return NATURAL_NR;}
-[1-9][0-9]* {return INTEGER_NR;}
([0-9]|[1-9][0-9]+).[0-9][0-9]* {return REAL_NR;}
\'[0-9a-zA-Z_=+\[\]{};':",./<>?-]\' {return CHAR;}
\"[0-9a-zA-Z_=+\[\]{};':",./<>?-]*\" {return STRING;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
