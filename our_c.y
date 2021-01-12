%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantics.h"


extern FILE* yyin;
extern char* yytext;
extern int yylineno;

int yylex();
void yyerror(const char *s);

struct scope *globalScope;

%}
%token ID TIP LOGICAL_OPERATOR ASSIGN INTEGER_NR NATURAL_NR REAL_NR CHAR STRING BOOL CLASS CONST OR AND IF ELSE WHILE
%start progr

%right ASSIGN
%left '+' '-'
%left '*' '/'
%left LOGICAL_OPERATOR
%left OR
%left AND
%left '!'

%union{
     int nr_i;
     char* string;
     char character;
     float nr_f;
     struct scope* scope_t;
     struct scope_entry* scope_entry_t;
     struct expr_type* expr_t;
     struct sign* sign_t;
}

%type <string> ID TIP LOGICAL_OPERATOR STRING CLASS CONST OR AND IF ELSE WHILE
%type <character> ASSIGN CHAR
%type <nr_i> INTEGER_NR NATURAL_NR BOOL
%type <nr_f> REAL_NR
%type <expr_t> expresie
%type <scope_t> progr declaratii bloc bloc_clasa
%type <sign_t> lista_semnatura membru_semnatura
%type <scope_entry_t> declaratie_var declaratie_func declaratie_clasa corp_declaratie atribuire_in_declaratie
%%
progr: declaratii {printf("%d : Done\n", yylineno); globalScope = $1;printf("program corect sintactic\n");}
     ;

declaratii: declaratie_var ';' { printf("%d : Declaratie variabila\n", yylineno); $$ = scopeFromEntry($1);}
          | declaratii declaratie_var ';' {printf("%d : Declaratii\n", yylineno); push($1, $2); $$ = $1;}
          | declaratie_func {printf("%d : Declaratie functie\n", yylineno); $$ = scopeFromEntry($1);}
          | declaratii declaratie_func {printf("%d : Declaratii functie\n", yylineno); push($1, $2); $$ = $1;}
          | declaratie_clasa ';' {printf("%d : Declaratie clasa\n", yylineno); $$ = scopeFromEntry($1);}
          | declaratii declaratie_clasa ';' {printf("%d : Declaratii clasa\n", yylineno); push($1, $2); $$ = $1;}
	     ;

declaratie_var: TIP corp_declaratie {printf("%d : Declaratie\n", yylineno); set_tip($2, $1); $$ = $2;}
          | CONST TIP atribuire_in_declaratie {printf("%d : Declaratie const\n", yylineno); $3->var.isConst = 1; $3->var.tip = strdup($2); $$ = $3;}
          | ID corp_declaratie  {set_tip($2, $1); $$ = $2;} 
          | CONST ID atribuire_in_declaratie {$3->var.isConst = 1; $3->var.tip = strdup($2); $$ = $3;}
          ;

declaratie_func: TIP ID '(' lista_semnatura ')' '{' bloc '}' {$$ = entry($1, $2, $4, $7);}
               | TIP ID '(' lista_semnatura ')' '{' '}' {$$ = entry($1, $2, $4);}
               | TIP ID '(' ')' '{' bloc '}' {$$ = entry($1, $2, $6);}
               | TIP ID '(' ')' '{' '}' {$$ = entry($1, $2);}
               | CONST TIP ID '(' lista_semnatura ')' '{' bloc '}' {$$ = entry($2, $3, $5, $8, 1);}
               | CONST TIP ID '(' lista_semnatura ')' '{' '}' {$$ = entry($2, $3, $5, 1);}
               | CONST TIP ID '(' ')' '{' bloc '}' {$$ = entry($2, $3, $7, 1);}
               | CONST TIP ID '(' ')' '{' '}' {$$ = entry($2, $3, 1);}
               | ID ID '(' lista_semnatura ')' '{' bloc '}' {$$ = entry($1, $2, $4, $7);}
               | ID ID '(' lista_semnatura ')' '{' '}' {$$ = entry($1, $2, $4);}
               | ID ID '(' ')' '{' bloc '}' {$$ = entry($1, $2, $6);}
               | ID ID '(' ')' '{' '}' {$$ = entry($1, $2);}
               | CONST ID ID '(' lista_semnatura ')' '{' bloc '}' {$$ = entry($2, $3, $5, $8, 1);}
               | CONST ID ID '(' lista_semnatura ')' '{' '}' {$$ = entry($2, $3, $5, 1);}
               | CONST ID ID '(' ')' '{' bloc '}' {$$ = entry($2, $3, $7, 1);}
               | CONST ID ID '(' ')' '{' '}' {$$ = entry($2, $3, 1);}
               ;

declaratie_clasa: CLASS ID '{' bloc_clasa '}' {$$ = classEntry($2, $4);}
               |  CLASS ID '{' '}' {$$ = classEntry($2);}
               ;

bloc:  declaratie_var ';' {$$ = scopeFromEntry($1);}
     | atribuire ';'
     | control
     | apel_functie ';'
     | bloc declaratie_var ';' {push($1, $2); $$ = $1;}
     | bloc atribuire ';'
     | bloc control
     | bloc apel_functie ';'
     ;

apel_functie: ID '(' lista_parametri ')'
          |   ID '(' ')'
          ;

lista_parametri: expresie
               | lista_parametri ',' expresie
               | atribuire
               | lista_parametri ',' atribuire
               | apel_functie
               | lista_parametri ',' apel_functie
               ;

control: if
     | while
     ;

if: IF '(' expresie ')' '{' bloc '}'
     | IF '(' expresie ')' '{' bloc '}' ELSE '{' bloc '}'
     ;

while: WHILE '(' expresie ')' '{' bloc '}'
     ;

bloc_clasa: declaratie_func {$$ = scopeFromEntry($1);}
          | declaratie_var ';' {$$ = scopeFromEntry($1);}
          | bloc_clasa declaratie_func {push($1, $2); $$ = $1;}
          | bloc_clasa declaratie_var ';' {push($1, $2), $$ = $1;}
          ;

lista_semnatura: membru_semnatura {$$ = $1;}
               | lista_semnatura ',' membru_semnatura {push($1, $3); $$ = $1;}
               ;

membru_semnatura: TIP ID {$$ = signEntry($1, $2);}
               |  CONST TIP ID {$$ = signEntry($2, $3, 1);}
               |  ID ID
               | CONST ID ID
               ;

corp_declaratie: ID {printf("%d : Corp declaratie\n", yylineno); $$ = entry($1);}
               | ID '[' NATURAL_NR ']' {printf("%d : Corp declaratie arr\n", yylineno); $$ = entry($1, $3);}
               | atribuire_in_declaratie {printf("%d : Corp declaratie atrib\n", yylineno); $$ = $1;}
               | corp_declaratie ',' ID {printf("%d : Corp declaratie multipla\n", yylineno); push($1, $3); $$ = $1;}
               | corp_declaratie ',' ID '[' NATURAL_NR ']' {push($1, $3, $5); $$ = $1;}
               | corp_declaratie ',' atribuire_in_declaratie {add($1, $3); $$ = $1;}
               ;

expresie:  REAL_NR {$$ = create_expr($1);}
          | INTEGER_NR {$$ = create_expr($1);}
          | NATURAL_NR {$$ = create_expr($1);}
          | BOOL {$$ = create_expr($1, 0);}
          | CHAR {$$ = create_expr($1);}
          | ID {}
          | expresie '+' expresie {$$ = expr_aritm($1, $3, $<character>2);}
          | expresie '-' expresie {$$ = expr_aritm($1, $3, $<character>2);}
          | expresie '*' expresie {$$ = expr_aritm($1, $3, $<character>2);}
          | expresie '/' expresie {$$ = expr_aritm($1, $3, $<character>2);}
          | '-' expresie {$2->val = -$2->val; $$ = $2;}
          | expresie AND expresie {$1->tip = 2; $1->val = $1->val && $3->val; $$ = $1;}
          | expresie OR expresie {$1->tip = 2; $1->val = $1->val || $3->val; $$ = $1;}
          | expresie LOGICAL_OPERATOR expresie {$$ = logical_operations($1, $3, $2);}
          | '(' expresie ')' {$$ = $2;}
          | '!' expresie {$2->tip = 2; $2->val = !$2->val; $$ = $2;}
          ;


atribuire_in_declaratie:   ID ASSIGN expresie {printf("%d : Atribuire expresie\n", yylineno); $$ = assign($1, $3);}
                         | ID ASSIGN STRING {printf("%d : Atribuire string\n", yylineno); $$ = assign($1, $3);}
                         ;

atribuire:  ID ASSIGN expresie {/*printf("%s = %f\n", $1, $3->val);*/}
          | ID ASSIGN STRING {/*printf("%s = %s\n", $1, $3);*/}
          | ID '[' NATURAL_NR ']' ASSIGN expresie {/*printf("%s[%i] = %f\n", $1, $3, $6->val);*/}
          | ID '[' NATURAL_NR ']' ASSIGN STRING {/*printf("%s[%i] = %s\n", $1, $3, $6);*/}
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");

     yyparse();

     display(globalScope);
} 