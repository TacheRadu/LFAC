%{
#include <stdio.h>
#include "our_c.tab.h"
%}
%option noyywrap
%%
"//".* ;
"int"|"char"|"string"|"bool"|"float" {return TIP;}
"class" {return CLASS;}
"const" {return CONST;}
"true"|"false" {return BOOL;}
"if" {return IF;}
"else" {return ELSE;}
"while" {return WHILE;};
"=="|"<"|">"|"<="|">=" {return LOGICAL_OPERATOR;}
"=" {return ASSIGN;}
"||" {return OR;}
"&&" {return AND;}
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
[0-9]|[1-9][0-9]+ {return NATURAL_NR;}
-[1-9][0-9]* {return INTEGER_NR;}
([0-9]|[1-9][0-9]+).[0-9][0-9]* {return REAL_NR;}
\'[0-9a-zA-Z_=+\[\]{};':",./<>?-]\' {return CHAR;}
\"[0-9a-zA-Z_=+\[\]{};':",./<>?-]*\" {return STRING;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
