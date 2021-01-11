#include "scope.h"

#define TAB_SIZE 5

void set_tip(struct scope_entry* e, char* tip){
    struct scope_entry* it = e;
    while(it != NULL){
        if(it->tip != 0){
            printf("%d: Habar n-am cum am ajuns aici\n", yylineno);
            exit(-1);
        }
        if(it->var.tip != NULL){
            if(strcmp(it->var.tip, tip) != 0){
                printf("%d: Invalid assignment value\n", yylineno);
                exit(-1);
            }
        }
        if(it->var.isArray){
            if(strcmp(tip, "int") == 0)
                it->var.iValArr = (int*) malloc(it->var.dim * sizeof(int));
            else if(strcmp(tip, "char") == 0)
                it->var.cValArr = (char*) malloc(it->var.dim * sizeof(char));
            else if(strcmp(tip, "string") == 0)
                it->var.sValArr = (char**) malloc(it->var.dim * sizeof(char*));
            else if(strcmp(tip, "bool") == 0)
                it->var.bValArr = (int*) malloc(it->var.dim * sizeof(int));
            else if(strcmp(tip, "float") == 0)
                it->var.fValArr = (float*) malloc(it->var.dim * sizeof(float));
            else
                it->var.classValArr = (int*) malloc(it->var.dim * sizeof(int));
        }
        it->var.tip = strdup(tip);
        it = it->next;
    }
}

void setValue(struct scope_entry *e, int n){
    if(strcmp(e->var.tip, "int") == 0){
        e->var.iValArr = (int*) malloc(n);
        return;
    }
    if(strcmp(e->var.tip, "char") == 0){
        e->var.cValArr = (char*) malloc(n);
        return;
    }
    if(strcmp(e->var.tip, "string") == 0){
        e->var.sValArr = (char**) malloc(n);
        return;
    }
    if(strcmp(e->var.tip, "bool") == 0){
        e->var.bValArr = (int*) malloc(n);
        return;
    }
    if(strcmp(e->var.tip, "float") == 0){
        e->var.fValArr = (float*) malloc(n);
        return;
    }
    e->var.classValArr = (int*) malloc(n);
}

bool sameSign(struct sign *a, struct sign *b){
    struct sign *it1 = a, *it2 = b;
    if((it1 == NULL && it2 != NULL) || (it1 != NULL && it2 == NULL))
        return 0;
    while(it1 != NULL && it2 != NULL){
        if(strcmp(it1->tip, it2->tip) != 0)
            return 0;
        it1 = it1->next;
        it2 = it2->next;
    }
    return 1;
}

void push(struct scope *s, struct scope_entry *e){
    struct scope_entry *it = s->first_item;
    if(it == NULL){
        s->first_item = e;
        return;
    }
    while(it->next != NULL){
        if((e->tip == 0 && (it->tip == 0 && strcmp(e->var.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->var.id, it->fun.id) == 0)) ||
           (e->tip == 1 && (it->tip == 0 && strcmp(e->fun.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->fun.id, it->fun.id) == 0)) && sameSign(e->fun.semnatura, it->fun.semnatura)){
            printf("%d: Redeclaration of variable\n", yylineno);
            exit(-1);
        }
        it = it->next;
    }
    it->next = e;
    e->prev = it;
    if(e->tip == 3)
        e->scope->prev = it;
}

void push(struct scope_entry *e, char* id){
    struct scope_entry* new_entry = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    new_entry->tip = 0;
    new_entry->var.tip = NULL;
    new_entry->var.id = strdup(id);
    new_entry->var.isArray = 0;
    struct scope_entry* it = e;
    while(it->next != NULL)
        it = it->next;
    it->next = new_entry;
    new_entry->prev = it;
    new_entry->next = NULL;
}

void push(struct scope_entry *e, char* id, int dim){
    struct scope_entry* new_entry = (struct scope_entry*) malloc(sizeof(struct scope_entry));
    new_entry->tip = 0;
    new_entry->var.id = strdup(id);
    new_entry->var.isArray = 1;
    new_entry->var.dim = dim;
    struct scope_entry* it = e;
    while(it->next != NULL)
        it = it->next;
    it->next = new_entry;
    new_entry->prev = it;
    new_entry->next = NULL;
}

void push(struct sign* t, struct sign* s){
    struct sign *it = t;
    while(it->next != NULL)
        it = it->next;
    it->next = s;
}

void add(struct scope_entry *t, struct scope_entry *s){
    struct scope_entry *it = t;
    while(it->next != NULL)
        it = it->next;
    it->next = s;
    s->prev = it;
}

void display(struct scope *s, int tabs = 0){
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        for(int i = 0; i < TAB_SIZE * tabs; i++)
            printf(" ");
        if(it->tip == 0){
            if(it->var.isConst)
                printf("const ");
            printf("%s %s;\n", it->var.tip, it->var.id);
        }
        if(it->tip == 1){
            printf("%s %s()\n", it->fun.tip, it->fun.id);
        }
        if(it->tip == 3){
            display(it->scope, tabs+1);
        }
        it = it->next;
    }
}
