%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIPi TIPd TIPc TIPs TIPb LOGICAL_OPERATOR MAIN ASSIGN NATURAL_NR REAL_NR CHAR STRING
%start progr
%%
progr: declaratii bloc {printf("program corect sintactic\n");}
     ;

declaratii:  declaratie ';'
	   | declaratii declaratie ';'
	   ;
declaratie: TIPi corp_declaratie_i
           ;
corp_declaratie_i: ID
               | ID ASSIGN NATURAL_NR
               | corp_declaratie_i ',' ID
               | corp_declaratie_i ',' ID ASSIGN NATURAL_NR
               ;
            
      
/* bloc */
bloc: TIPi MAIN '(' ')' '{' list '}'
     ;
     
/* lista instructiuni */
list:  statement ';' 
     | list statement ';'
     ;

/* instructiune */
statement: atribuire 
         ;

atribuire: ID ASSIGN NATURAL_NR
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 