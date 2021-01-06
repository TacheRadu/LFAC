#include <assert.h>
#include <stdlib.h> /* malloc, realloc. */
#include <string.h> /* strlen. */

#define VAR 0
#define FUN 1

/* Data type for links in the chain of symbols. */
struct symrec{
    char *name; /* name of symbol */
    char *data_type;
    int type; /* type of symbol: either VAR or FUN */
    union{
        int iVal;
        char cVal;
        float fVal;
        double dVal;
        bool bVal;
        char *sVal;
    } value;
    struct symrec *next; /* link field */
};

typedef struct symrec symrec;

/* The symbol table: a chain of 'struct symrec'. */
extern symrec *sym_table;

symrec *getsym(char const *name){
    for (symrec *p = sym_table; p; p = p->next)
        if (strcmp(p->name, name) == 0)
            return p;
    return NULL;
}
