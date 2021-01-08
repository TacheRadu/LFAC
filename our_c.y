%{
#include <stdio.h>
#include <stdlib.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIP LOGICAL_OPERATOR MAIN ASSIGN INTEGER_NR NATURAL_NR REAL_NR CHAR STRING BOOL CLASS CONST OR AND
%start progr

%right ASSIGN
%left '+' '-'
%left '*' '/'
%left LOGICAL_OPERATOR
%left OR
%left AND
%left '!'
%%
progr: declaratii bloc {printf("program corect sintactic\n");}
     ;

declaratii:  declaratie ';'
	     | declaratii declaratie ';'
	     ;

declaratie: TIP corp_declaratie
          | CONST TIP atribuire_in_declaratie
          | ID corp_declaratie  {printf("Data type not defined\n(Line %d)\n", yylineno); exit('0' - '1');} 
          | CONST ID atribuire_in_declaratie
          ;

corp_declaratie: ID
               | ID '[' NATURAL_NR ']'
               | atribuire_in_declaratie
               | corp_declaratie ',' ID
               | corp_declaratie ',' ID '[' NATURAL_NR ']'
               | corp_declaratie ',' atribuire_in_declaratie
               ;

expression:  REAL_NR
          | INTEGER_NR
          | NATURAL_NR
          | BOOL
          | CHAR
          | ID
          | expression '+' expression
          | expression '-' expression
          | expression '*' expression
          | expression '/' expression
          | '-' expression
          | expression AND expression
          | expression OR expression
          | expression LOGICAL_OPERATOR expression
          | '(' expression ')'
          | '!' expression
          ;

/* bloc */
bloc: TIP MAIN '(' ')' '{' list '}'
     ;
     
/* lista instructiuni */
list:  statement ';' 
     | list statement ';'
     ;

/* instructiune */
statement: atribuire 
         ;

atribuire_in_declaratie:   ID ASSIGN expression
                         | ID ASSIGN STRING
                         ;

atribuire:  ID ASSIGN expression
          | ID ASSIGN STRING
          | ID '[' NATURAL_NR ']' ASSIGN expression
          | ID '[' NATURAL_NR ']' ASSIGN STRING
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 