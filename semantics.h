#include "scope_entry.h"

#define TAB_SIZE 5

void set_tip(struct scope_entry* e, char* tip){
    struct scope_entry* it = e;
    while(it != NULL){
        if(it->tip == 0 && it->var.tip != NULL){
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
                it->var.bValArr = (bool*) malloc(it->var.dim * sizeof(bool));
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
        e->var.iValArr = (int*) malloc(n * sizeof(int));
        return;
    }
    if(strcmp(e->var.tip, "char") == 0){
        e->var.cValArr = (char*) malloc(n * sizeof(char));
        return;
    }
    if(strcmp(e->var.tip, "string") == 0){
        e->var.sValArr = (char**) malloc(n * sizeof(char*));
        return;
    }
    if(strcmp(e->var.tip, "bool") == 0){
        e->var.bValArr = (bool*) malloc(n * sizeof(bool));
        return;
    }
    if(strcmp(e->var.tip, "float") == 0){
        e->var.fValArr = (float*) malloc(n * sizeof(float));
        return;
    }
    e->var.classValArr = (int*) malloc(n * sizeof(int));
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

void printSign(struct scope_entry *e){
    struct sign *it = e->fun.semnatura;
    while(it != NULL){
        printf("%s %s", it->tip, it->id);
        if(it->next != NULL)
            printf(", ");
        it = it->next;
    }
}

void display(struct scope *s, int tabs = 0){
    if(s == NULL)
        return;
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
            printf("%s %s(", it->fun.tip, it->fun.id);
            printSign(it);
            printf("){\n");
            display(it->fun.scope, tabs + 1);
            for(int i = 0; i < TAB_SIZE * tabs; i++)
                printf(" ");
            printf("}\n");
        }
        if(it->tip == 2){
            printf("class %s{\n", it->cls.id);
            display(it->cls.scope, tabs + 1);
            for(int i = 0; i < TAB_SIZE * tabs; i++)
                printf(" ");
            printf("};\n");
        }
        if(it->tip == 3){
            printf("%s %s = smth\n", it->assignment.var->var.tip, it->assignment.var->var.id);
        }
        it = it->next;
    }
}

bool notStdType(char *tip){
    if(strcmp(tip, "int") == 0)
        return 0;
    if(strcmp(tip, "char") == 0)
        return 0;
    if(strcmp(tip, "string") == 0)
        return 0;
    if(strcmp(tip, "bool") == 0)
        return 0;
    if(strcmp(tip, "float") == 0)
        return 0;
    return 1;
}

bool lookForClass(struct scope_entry *v){
    struct scope_entry *it = v->prev;
    while(it != NULL){
        if(it->tip == 2 &&
          (v->tip == 0 && strcmp(it->cls.id, v->var.tip) == 0) ||
          (v->tip == 1 && strcmp(it->cls.id, v->fun.tip) == 0))
          return 1;
        it = it->prev;
    }
    return 0;

}

void checkSign(struct scope_entry *e){
    struct sign *s = e->fun.semnatura;
    while(s != NULL){
        bool f = 0;
        struct scope_entry *it = e->prev;
        while(it != NULL){
            if(!notStdType(s->tip))
                f = 1;
            if(it->tip == 2 && strcmp(it->cls.id, s->tip) == 0){
                f = 1;
                break;
            }
            it = it->prev;
        }
        if(!f){
            printf("%s: No such type\n", s->tip);
            exit(-1);
        }
        s = s->next;
    }
}

void check(struct scope *s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        // check type of variables and functions
        if((it->tip == 0 && notStdType(it->var.tip) && !lookForClass(it)) ||
           (it->tip == 1 && notStdType(it->fun.tip) && !lookForClass(it))){
            printf("%s: No such type\n", it->var.tip);
            exit(-1);
        }

        //check type of function parameters;
        if(it->tip == 1)
            checkSign(it);

        it = it->next;
    }
}
