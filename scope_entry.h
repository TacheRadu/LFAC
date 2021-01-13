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
    // type = 3: ASSIGNMENT
    union{
        struct {
            char *tip;
            char *id;
            bool isArray;
            bool isConst;
            int dim;
            union{
                bool isSet;
                bool *isSetElem;
            };
            union{
                int iVal;
                int *iValArr;
                char cVal;
                char *cValArr;
                char *sVal;
                char **sValArr;
                bool bVal;
                bool *bValArr;
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
            struct scope* scope;
        }fun;
        struct {
            char *id;
            struct scope* scope;
        }cls;
        struct {
            char* id;
            struct expr_type *expression;
            struct scope_entry *var;
            bool isArray;
            int index;
        }assignment;
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
    e->prev = NULL;
    e->next = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = semnatura;
    e->fun.isConst = isConst;
    e->fun.scope = bloc;
    struct sign *param = e->fun.semnatura;
    while(param != NULL){
        struct scope_entry *vars = e->fun.scope->first_item;
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
    e->prev = NULL;
    e->next = NULL;
    e->tip = 1;
    e->fun.tip = strdup(tip);
    e->fun.id = strdup(id);
    e->fun.semnatura = NULL;
    e->fun.isConst = isConst;
    e->fun.scope = bloc;
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
    e->fun.scope = NULL;
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
    e->fun.scope = NULL;
    e->fun.isConst = isConst;
    return e;
}

struct scope_entry* classEntry(char* id){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->tip = 2;
    e->cls.id = strdup(id);
    e->cls.scope = NULL;
    return e;
}

struct scope_entry* classEntry(char* id, struct scope* scope){
    struct scope_entry* e = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    e->prev = NULL;
    e->next = NULL;
    e->tip = 2;
    e->cls.id = strdup(id);
    e->cls.scope = scope;
    printf("In functiune\n");
    return e;
}

struct scope_entry* decl_assign_entry(char* id, struct expr_type *exp, bool isConst = 0, char* tip = NULL){
    struct scope_entry *e = entry(id);
    e->var.isConst = isConst;
    if(tip != NULL)
        e->var.tip = strdup(tip);
    struct scope_entry *a = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    a->tip = 3;
    a->next = NULL;
    a->assignment.id = strdup(id);
    a->assignment.var = e;
    a->assignment.expression = exp;
    a->assignment.isArray = false;
    e->next = a;
    a->prev = e;
    return e;
}

struct scope_entry* assign_entry(char* id, struct expr_type *exp){
    struct scope_entry *a = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    a->tip = 3;
    a->prev = NULL;
    a->next = NULL;
    a->assignment.id = strdup(id);
    a->assignment.isArray = false;
    a->assignment.var = NULL;
    a->assignment.expression = exp;
    return a;
}

struct scope_entry* assign_entry(char* id, int index, struct expr_type *exp){
    struct scope_entry *a = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    a->tip = 3;
    a->prev = NULL;
    a->next = NULL;
    a->assignment.id = strdup(id);
    a->assignment.var = NULL;
    a->assignment.isArray = true;
    a->assignment.index = index;
    a->assignment.expression = exp;
    return a;
}