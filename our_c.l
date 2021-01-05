%{
#include <stdio.h>
#include "our_c.tab.h"
%}
%option noyywrap
%%
"int"|"float"|"char"|"string"|"bool" {return TIP;}
"main" {return MAIN;}
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
"=" {return ASSIGN;}
"=="|"<"|">"|"<="|">="|"||"|"&&" {return LOGICAL_OPERATOR;}
[0-9]|[1-9][0-9]+ {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
