%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIPi TIPd TIPc TIPs TIPb LOGICAL_OPERATOR MAIN ASSIGN NATURAL_NR REAL_NR CHAR STRING BOOL
%start progr
%%
progr: declaratii bloc {printf("program corect sintactic\n");}
     ;

declaratii:  declaratie ';'
	   | declaratii declaratie ';'
	   ;
declaratie: TIPi corp_declaratie_i 
          | TIPc corp_declaratie_c
          | TIPd corp_declaratie_d
          | TIPs corp_declaratie_s
          | TIPb corp_declaratie_b
           ;
corp_declaratie_i: ID
               | ID ASSIGN NATURAL_NR
               | corp_declaratie_i ',' ID
               | corp_declaratie_i ',' ID ASSIGN NATURAL_NR
               ;
corp_declaratie_c: ID
               | ID ASSIGN CHAR
                | corp_declaratie_c ',' ID
               | corp_declaratie_c ',' ID ASSIGN CHAR
               ;
corp_declaratie_d: ID
               | ID ASSIGN REAL_NR
                | corp_declaratie_d ',' ID
               | corp_declaratie_d ',' ID ASSIGN REAL_NR
               ;
corp_declaratie_s: ID
               | ID ASSIGN STRING
                | corp_declaratie_s ',' ID
               | corp_declaratie_s ',' ID ASSIGN STRING
               ;
corp_declaratie_b: ID
               | ID ASSIGN BOOL
               | ID ASSIGN REAL_NR
               | ID ASSIGN NATURAL_NR
               | corp_declaratie_b ',' ID
               | corp_declaratie_b ',' ID ASSIGN BOOL
               | corp_declaratie_b ',' ID ASSIGN REAL_NR
               | corp_declaratie_b ',' ID ASSIGN NATURAL_NR
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