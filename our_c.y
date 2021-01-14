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
     bool bool_t;
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
%type <nr_i> INTEGER_NR NATURAL_NR
%type <bool_t> BOOL
%type <nr_f> REAL_NR
%type <expr_t> expresie
%type <scope_t> progr declaratii bloc bloc_clasa
%type <sign_t> lista_semnatura membru_semnatura
%type <scope_entry_t> declaratie_var declaratie_func declaratie_clasa corp_declaratie atribuire control if while
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
          | CONST TIP ID ASSIGN expresie {printf("%d : Atribuire expresie\n", yylineno); $$ = decl_assign_entry($3, $5, 1, $2);}
          | ID corp_declaratie  {set_tip($2, $1); $$ = $2;} 
          | CONST ID ID ASSIGN expresie {printf("%d : Atribuire expresie\n", yylineno); $$ = decl_assign_entry($3, $5, 1, $2);}
          ;

declaratie_func: TIP ID '(' lista_semnatura ')' '{' bloc '}' {$$ = entry($1, $2, $4, $7); printf("Dupa ce a facut entry\n");}
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

declaratie_clasa: CLASS ID '{' bloc_clasa '}' {$$ = classEntry($2, $4); printf("Am facut si classEntry\n");}
               |  CLASS ID '{' '}' {$$ = classEntry($2);}
               ;

bloc:  declaratie_var ';' {$$ = scopeFromEntry($1);}
     | atribuire ';' {$$ = scopeFromEntry($1);}
     | control {$$ = scopeFromEntry($1);}
     | apel_functie ';'
     | bloc declaratie_var ';' {push($1, $2); $$ = $1;}
     | bloc atribuire ';' {push($1, $2); $$ = $1;}
     | bloc control {push($1, $2); $$ = $1;}
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

control: if {$$ = $1;}
     | while {$$ = $1;}
     ;

if: IF '(' expresie ')' '{' bloc '}' {$$ = ifEntry($3, $6);}
     | IF '(' expresie ')' '{' bloc '}' ELSE '{' bloc '}' {$$ = ifEntry($3, $6, $10);}
     ;

while: WHILE '(' expresie ')' '{' bloc '}' {$$ = whileEntry($3, $6);}
     ;

bloc_clasa: declaratie_func {$$ = scopeFromEntry($1); printf("Facem si asta\n");}
          | declaratie_var ';' {$$ = scopeFromEntry($1);}
          | bloc_clasa declaratie_func {push($1, $2); $$ = $1; printf("Push-ul stie sa il faca\n");}
          | bloc_clasa declaratie_var ';' {push($1, $2), $$ = $1;}
          ;

lista_semnatura: membru_semnatura {$$ = $1;}
               | lista_semnatura ',' membru_semnatura {push($1, $3); $$ = $1;}
               ;

membru_semnatura: TIP ID {$$ = signEntry($1, $2);}
               |  CONST TIP ID {$$ = signEntry($2, $3, 1);}
               |  ID ID {$$ = signEntry($1, $2);}
               | CONST ID ID {$$ = signEntry($2, $3, 1);}
               ;

corp_declaratie: ID {printf("%d : Corp declaratie\n", yylineno); $$ = entry($1);}
               | ID '[' NATURAL_NR ']' {printf("%d : Corp declaratie arr\n", yylineno); $$ = entry($1, $3);}
               | ID ASSIGN expresie {printf("%d : Atribuire expresie\n", yylineno); $$ = decl_assign_entry($1, $3);}
               | corp_declaratie ',' ID {printf("%d : Corp declaratie multipla\n", yylineno); push($1, $3); $$ = $1;}
               | corp_declaratie ',' ID '[' NATURAL_NR ']' {push($1, $3, $5); $$ = $1;}
               | corp_declaratie ',' ID ASSIGN expresie {add($1, decl_assign_entry($3, $5)); $$ = $1;}
               ;

expresie:  REAL_NR {$$ = create_expr($1);}
          | INTEGER_NR {$$ = create_expr($1);}
          | NATURAL_NR {$$ = create_expr($1);}
          | BOOL {$$ = create_expr($1);}
          | CHAR {$$ = create_expr($1);}
          | STRING {$$ = create_expr($1, true);}
          | ID {$$ = create_expr($1);}
          | ID '[' NATURAL_NR ']' {$$ = create_expr($1, $3);}
          | expresie '+' expresie {$$ = expr($1, $3, 1);}
          | expresie '-' expresie {$$ = expr($1, $3, 2);}
          | expresie '*' expresie {$$ = expr($1, $3, 3);}
          | expresie '/' expresie {$$ = expr($1, $3, 4);}
          | '-' expresie {$$ = expr_negate($2);}
          | expresie AND expresie {$$ = expr($1, $3, 5);}
          | expresie OR expresie {$$ = expr($1, $3, 6);}
          | expresie LOGICAL_OPERATOR expresie {$$ = expr($1, $3, $2);}
          | '(' expresie ')' {$$ = $2;}
          | '!' expresie {$$ = expr_negation($2);}
          ;

atribuire:  ID ASSIGN expresie {printf("%d: I do assign\n", yylineno); $$ = assign_entry($1, $3);}
          | ID '[' NATURAL_NR ']' ASSIGN expresie {$$ = assign_entry($1, $3, $6);}
          ;
%%
void yyerror(const char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");
     // redeclarations are checked here, since declarations are scope-dependent
     yyparse();

     //link the prevs of the first elements of scopes
     linkBlocPrevs(globalScope);
     // check if declarations of a certain type are valid.
     checkDeclarations(globalScope);
     // checks if the types of the parameters actually exist.
     // also check that there is only one definition of a function with a specific signature
     checkFunctionSignatures(globalScope);
     // check if the variable that is assigned to was previously defined
     checkAssignments(globalScope);
     // link the id with the corresponding variables
     // also here check if variables in expression exist. If they do, link them as well
     setAssignments(globalScope);

     display(globalScope);
} 