%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

%}
%token ID TIP LOGICAL_OPERATOR MAIN ASSIGN NR 
%start progr
%%
progr: declaratii bloc {printf("program corect sintactic\n");}
     ;

declaratii:  declaratie ';'
	   | declaratii declaratie ';'
	   ;
declaratie: TIP corp_declaratie
           ;
corp_declaratie: ID
               | ID ASSIGN NR
               | corp_declaratie ',' ID
               | corp_declaratie ',' ID ASSIGN NR 
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

atribuire: ID ASSIGN NR
          | ID ASSIGN atribuire;
        
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 