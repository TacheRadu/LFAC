#include <stdlib.h> /* malloc, realloc. */
#include <string.h> /* strlen. */
#include <stdio.h>

#include "expr_type.h"

struct scope_entry{
    struct scope_entry *prev, *next;
    int tip;
    // type = 0: VAR
    // type = 1: FUN
    // type = 2: CLASS
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
            struct scope *context;
        }fun;
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