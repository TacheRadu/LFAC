%{
#include <stdio.h>
#include <stdlib.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIP LOGICAL_OPERATOR MAIN ASSIGN INTEGER_NR NATURAL_NR REAL_NR CHAR STRING BOOL CLASS CONST
%start progr
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

atribuire_in_declaratie:   ID ASSIGN INTEGER_NR
                         | ID ASSIGN NATURAL_NR
                         | ID ASSIGN REAL_NR
                         | ID ASSIGN BOOL
                         | ID ASSIGN CHAR
                         | ID ASSIGN STRING
                         | ID ASSIGN ID
                         ;

atribuire:  ID ASSIGN INTEGER_NR
          | ID ASSIGN NATURAL_NR
          | ID ASSIGN REAL_NR
          | ID ASSIGN BOOL
          | ID ASSIGN CHAR
          | ID ASSIGN STRING
          | ID ASSIGN ID
          | ID '[' NATURAL_NR ']' ASSIGN INTEGER_NR
          | ID '[' NATURAL_NR ']' ASSIGN NATURAL_NR
          | ID '[' NATURAL_NR ']' ASSIGN REAL_NR
          | ID '[' NATURAL_NR ']' ASSIGN BOOL
          | ID '[' NATURAL_NR ']' ASSIGN CHAR
          | ID '[' NATURAL_NR ']' ASSIGN STRING
          | ID '[' NATURAL_NR ']' ASSIGN ID
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 