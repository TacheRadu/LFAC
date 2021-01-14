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
        if(it->tip == 0)
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
    if((e->tip == 0 && (it->tip == 0 && strcmp(e->var.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->var.id, it->fun.id) == 0)) ||
       (e->tip == 1 && (it->tip == 0 && strcmp(e->fun.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->fun.id, it->fun.id) == 0 && sameSign(e->fun.semnatura, it->fun.semnatura)))){
        printf("%d: Redeclaration of variable\n", yylineno);
        exit(-1);
    }
    while(it->next != NULL){
        if((e->tip == 0 && (it->tip == 0 && strcmp(e->var.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->var.id, it->fun.id) == 0)) ||
           (e->tip == 1 && (it->tip == 0 && strcmp(e->fun.id, it->var.id) == 0 || it->tip == 1 && strcmp(e->fun.id, it->fun.id) == 0 && sameSign(e->fun.semnatura, it->fun.semnatura)))){
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
    struct scope_entry* new_entry = entry(id, dim);
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
            printf("%s", it->assignment.var->var.id);
            if(it->assignment.isArray)
                printf("[%d]", it->assignment.index);
            printf(" = smth\n");
        }
        if(it->tip == 4){
            printf("if(){\n");
            display(it->if_s.scope, tabs + 1);
            for(int i = 0; i < TAB_SIZE * tabs; i++)
                printf(" ");
            printf("}\n");
            if(it->if_s.else_scope != NULL){
                for(int i = 0; i < TAB_SIZE * tabs; i++)
                    printf(" ");
                printf("else{\n");
                display(it->if_s.else_scope, tabs + 1);
                for(int i = 0; i < TAB_SIZE * tabs; i++)
                    printf(" ");
                printf("}\n");
            }
        }
        if(it->tip == 5){
            printf("while(){\n");
            display(it->while_s.scope, tabs + 1);
            for(int i = 0; i < TAB_SIZE * tabs; i++)
                printf(" ");
            printf("}\n");
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
            if(!notStdType(s->tip)){
                f = 1;
                break;
            }
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

void checkFunctionDefinition(struct scope_entry *e){
    struct scope_entry *it = e->prev;
    while(it != NULL){
        if(it->tip == 1 && strcmp(e->fun.id, it->fun.id) == 0 && sameSign(it->fun.semnatura, e->fun.semnatura)){
            printf("%s: Cannot redefine function with the same signature\n", e->fun.id);
            exit(-1);
        }
        it = it->prev;
    }
}

void checkFunctionSignatures(struct scope *s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        if(it->tip == 1){
            checkSign(it);
            checkFunctionDefinition(it);
        }
        if(it->tip == 2)
            checkFunctionSignatures(it->cls.scope);

        it = it->next;
    }
}

void checkDeclarations(struct scope *s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        // check type of variables
        if(it->tip == 0 && notStdType(it->var.tip) && !lookForClass(it)){
            printf("%s: No such type\n", it->var.tip);
            exit(-1);
        }

        //check type of nested variables
        if(it->tip == 1){
            checkDeclarations(it->fun.scope);
        }

        it = it->next;
    }
}

void checkVariable(struct scope_entry *e){
    struct scope_entry *it = e->prev;
    bool f = 0;
    while(it != NULL){
        if(it->tip == 0 && strcmp(e->assignment.id, it->var.id) == 0){
            if(it->var.isArray && !e->assignment.isArray){
                printf("%s: cannot assign expression to an array type\n", it->var.id);
                exit(-1);
            }
            if(e->assignment.isArray && !it->var.isArray){
                printf("%s: cannot assign element of non-array type\n", it->var.id);
                exit(-1);
            }
            f = 1;
            break;
        }
        it = it->prev;
    }
    if(!f){
        printf("%s: undeclared variable\n", e->assignment.id);
        exit(-1);
    }
}

void checkAssignments(struct scope *s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        if(it->tip == 3)
            checkVariable(it);
        if(it->tip == 1)
            checkAssignments(it->fun.scope);
        if(it->tip == 2)
            checkAssignments(it->cls.scope);
        if(it->tip == 4){
            checkAssignments(it->if_s.scope);
            checkAssignments(it->if_s.else_scope);
        }
        if(it->tip == 5)
            checkAssignments(it->while_s.scope);

        it = it->next;
    }
}

void setAssignment(struct scope_entry *a){
    struct scope_entry *it = a->prev;
    while(it != NULL){
        if(it->tip == 0 && strcmp(it->var.id, a->assignment.id) == 0){
            a->assignment.var = it;
            if(it->var.isConst && it->var.isSet){
                printf("%s: cannot assign to constant value\n", it->var.id);
                exit(-1);
            }
            if(it->var.isArray){
                if(a->assignment.index >= it->var.dim){
                    printf("%s[%d]: Out of bounds index\n", it->var.id, a->assignment.index);
                    exit(-1);
                }
                it->var.isSetElem[a->assignment.index] = true;
            }
            if(!it->var.isArray)
                it->var.isSet = true;
            return;
        }
        it = it->prev;
    }
}

void checkExpression(struct expr_type *e, struct scope_entry *a){
    if(e->isVar){
        struct scope_entry *it = a->prev;
        bool f = 0;
        while(it != NULL){
            if(it->tip == 0 && strcmp(it->var.id, e->var.id) == 0){
                f = 1;
                if(it->var.isArray && !e->var.isArr){
                    printf("%s: cannot assign array value in expression\n", it->var.id);
                    exit(-1);
                }
                if(!it->var.isArray && e->var.isArr){
                    printf("%s: not an array\n", it->var.id);
                    exit(-1);
                }
                if(it->var.isArray && it->var.dim <= e->var.indexNo){
                    printf("%s[%d]: out of bounds index\n", it->var.id, e->var.indexNo);
                    exit(-1);
                }
                if(it->var.isArray && !it->var.isSetElem[e->var.indexNo] || !it->var.isArray && !it->var.isSet){
                    printf("%s: cannot use uninitialized variable in expression\n", it->var.id);
                    exit(-1);
                }
                e->var.var = it;
                if(strcmp(it->var.tip, "string") == 0){
                    e->isString = true;
                }
                break;
            }
            it = it->prev;
        }
        if(!f){
            printf("%s: undefined variable\n", e->var.id);
            exit(-1);
        }
    }
    if(!e->isVar && !e->isString && e->exp.left != NULL){
        if(e->exp.op == 12)
            checkExpression(e->exp.left, a);
        else{
            checkExpression(e->exp.left, a);
            checkExpression(e->exp.right, a);
        }
    }

}

void setAssignments(struct scope *s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        if(it->tip == 3){
            setAssignment(it);
            checkExpression(it->assignment.expression, it);
        }
        if(it->tip == 1)
            setAssignments(it->fun.scope);
        if(it->tip == 2)
            setAssignments(it->cls.scope);
        if(it->tip == 4){
            setAssignments(it->if_s.scope);
            setAssignments(it->if_s.else_scope);
        }
        if(it->tip == 5)
            setAssignments(it->while_s.scope);
        it = it->next;
    }
}

void linkBlocPrevs(struct scope* s){
    if(s == NULL)
        return;
    struct scope_entry *it = s->first_item;
    while(it != NULL){
        if(it->tip == 1 && it->fun.scope != NULL){
            it->fun.scope->first_item->prev = it->prev;
            linkBlocPrevs(it->fun.scope);
        }
        if(it->tip == 2 && it->cls.scope != NULL){
            it->cls.scope->first_item->prev = it->prev;
            linkBlocPrevs(it->cls.scope);
        }
        if(it->tip == 4 && it->if_s.scope != NULL){
            it->if_s.scope->first_item->prev = it->prev;
            linkBlocPrevs(it->if_s.scope);
            if(it->if_s.else_scope != NULL){
                it->if_s.else_scope->first_item->prev = it->prev;
                linkBlocPrevs(it->if_s.else_scope);
            }
        }
        if(it->tip == 5 && it->while_s.scope != NULL){
            it->while_s.scope->first_item->prev = it->prev;
            linkBlocPrevs(it->while_s.scope);
        }
        it = it->next;
    }

}
