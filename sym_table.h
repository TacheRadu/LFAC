#include <assert.h>
#include <stdlib.h> /* malloc, realloc. */
#include <string.h> /* strlen. */
#include <stdio.h>

extern int yylineno;
struct sign{
    char *tip;
    struct sign *next;
};

struct expr_type{
    int tip;
    // int: 0
    // char: 1
    // bool: 2
    // float: 3
    
    float val;
};

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


struct scope{
    struct scope *prev;
    struct scope_entry *first_item;
};