#include <stdlib.h> /* malloc, realloc. */
#include <string.h> /* strlen. */
#include <stdio.h>

#include "expr_type.h"
#include "scope.h"
#include "sign.h"

struct scope_entry{
    struct scope_entry *prev, *next;
    int tip;
    // type = 0: VAR
    // type = 1: FUN
    // type = 2: CLASS
    // type = 3: SCOPE
    union{
        struct {
            char *tip;
            char *id;
            int isArray;
            int isConst;
            int dim;
            union{
                int iVal;
                int *iValArr;
                char cVal;
                char *cValArr;
                char *sVal;
                char **sValArr;
                int bVal;
                int *bValArr;
                float fVal;
                float *fValArr;
                int classVal;
                int *classValArr;
            };
        }var;
        struct {
            char *tip;
            char *id;
            struct sign *semnatura;
            bool isConst;
        }fun;
        struct scope* scope;
    };
};

struct scope_entry* entry(char* id){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->var.id = strdup(id);
    e->var.tip = NULL;
    e->tip = 0;
    e->var.isArray = 0;
    return e;
}
struct scope_entry* entry(char* id, int dim){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->var.id = strdup(id);
    e->var.tip = NULL;
    e->tip = 0;
    e->var.isArray = 1;
    e->var.dim = dim;
    return e;
}

struct scope_entry* entry(char* tip, char* id, struct sign* semnatura, struct scope* bloc, bool isConst = 0){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    struct scope_entry* e_bloc = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e_bloc->next = NULL;
    e_bloc->tip = 3;
    e_bloc->scope = bloc;
    e->prev = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = semnatura;
    e->fun.isConst = isConst;
    e->next = e_bloc;
    e_bloc->prev = e;
    struct sign *param = e->fun.semnatura;
    while(param != NULL){
        struct scope_entry *vars = e_bloc->scope->first_item;
        while(vars != NULL){
            if(vars->tip == 0 && strcmp(param->id, vars->var.id) == 0){
                printf("%d: Variable declaration shadows a parameter\n", yylineno);
                exit(-1);
            }
            vars = vars->next;
        }
        param = param->next;
    }
    return e;
}

struct scope_entry* entry(char* tip, char* id, struct scope* bloc, bool isConst = 0){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    struct scope_entry* e_bloc = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e_bloc->next = NULL;
    e_bloc->tip = 3;
    e_bloc->scope = bloc;
    e->prev = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = NULL;
    e->fun.isConst = isConst;
    e->next = e_bloc;
    e_bloc->prev = e;
    return e;
}

struct scope_entry* entry(char* tip, char* id, bool isConst = 0){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = NULL;
    e->fun.isConst = isConst;
    return e;
}

struct scope_entry* entry(char* tip, char* id, struct sign *semnatura, bool isConst = 0){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = semnatura;
    e->fun.isConst = isConst;
    return e;
}

struct scope_entry* assign(char* id, struct expr_type *exp){
    struct scope_entry* res = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    res->tip = 0;
    res->var.id = strdup(id);
    res->var.isArray = 0;
    res->next = NULL;
    res->prev = NULL;
    switch(exp->tip){
        case 0:
            res->var.tip = strdup("int");
            res->var.iVal = (int) exp->val;
            break;
        case 1:
            res->var.tip = strdup("char");
            res->var.cVal = (char) exp->val;
            break;
        case 2:
            res->var.tip = strdup("bool");
            res->var.bVal = (int) exp->val;
            break;
        case 3:
            res->var.tip = strdup("float");
            res->var.fVal = exp->val;
    }
    return res;
}

struct scope_entry* assign(char* id, char* str){
    struct scope_entry* res = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    res->tip = 0;
    res->var.sVal = strdup(str);
    res->var.id = strdup(id);
    res->next = NULL;
    res->prev = NULL;
    return res;
}