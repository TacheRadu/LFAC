%{
#include <stdio.h>
#include <stdlib.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIP LOGICAL_OPERATOR MAIN ASSIGN INTEGER_NR NATURAL_NR REAL_NR CHAR STRING BOOL CLASS
%start progr
%%
progr: declaratii bloc {printf("program corect sintactic\n");}
     ;

declaratii:  declaratie ';'
	   | declaratii declaratie ';'
	   ;
declaratie: TIP corp_declaratie 
          | ID corp_declaratie  {printf("Data type not defined\n(Line %d)\n", yylineno); exit('0' - '1');} 
           ;
corp_declaratie: ID
               | atribuire
               | corp_declaratie ',' ID
               | corp_declaratie ',' atribuire
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

atribuire:  ID ASSIGN INTEGER_NR
          | ID ASSIGN NATURAL_NR
          | ID ASSIGN REAL_NR
          | ID ASSIGN BOOL
          | ID ASSIGN CHAR
          | ID ASSIGN STRING
          | ID ASSIGN ID
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 