%{
#include <stdio.h>
#include "our_c.tab.h"
%}
%option noyywrap
%%
"//".* ;
"int"|"char"|"string"|"bool"|"float" {yylval.string = strdup(yytext); return TIP;}
"class" {yylval.string = strdup(yytext); return CLASS;}
"const" {yylval.string = strdup(yytext); return CONST;}
"true"|"false" {if(strcmp(yytext, "true") == 0) yylval.nr_i = 1; else yylval.nr_i = 0; return BOOL;}
"if" {yylval.string = strdup(yytext); return IF;}
"else" {yylval.string = strdup(yytext); return ELSE;}
"while" {yylval.string = strdup(yytext); return WHILE;};
"=="|"<"|">"|"<="|">=" {yylval.string = strdup(yytext); return LOGICAL_OPERATOR;}
"=" {yylval.character = yytext[0]; return ASSIGN;}
"||" {yylval.string = strdup(yytext); return OR;}
"&&" {yylval.string = strdup(yytext); return AND;}
[_a-zA-Z][_a-zA-Z0-9]* {yylval.string = strdup(yytext); return ID;}
[0-9]|[1-9][0-9]+ {yylval.nr_i = atoi(yytext); return NATURAL_NR;}
-[1-9][0-9]* {yylval.nr_i = atoi(yytext); return INTEGER_NR;}
([0-9]|[1-9][0-9]+).[0-9][0-9]* {yylval.nr_f = atof(yytext); return REAL_NR;}
\'[0-9a-zA-Z_=+\[\]{};':",./<>?-]\' {yylval.character = yytext[1]; return CHAR;}
\"[0-9a-zA-Z_=+\[\]{};':",./<>?-]*\" {yylval.string = strdup(yytext); return STRING;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
