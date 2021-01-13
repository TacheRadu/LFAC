#define max(X, Y) (((X) < (Y)) ? (X) : (Y))

extern int yylineno;

struct expr_type{
    char *tip;
    bool isVar;
    bool isString;
    union{
        struct {
            bool isArr;
            int indexNo;
            char *id;
            struct scope_entry *var;
        }var;
        struct {
            struct expr_type *left, *right;
            int op;
            // +: 1
            // -: 2
            // *: 3
            // /: 4
            // AND: 5
            // OR: 6
            // ==: 7
            // <: 8
            // <=: 9
            // >: 10
            // >=: 11
            // !: 12
        }exp;
    };
    union{
        float val;
        char* sVal;
    };
};

struct expr_type* create_expr(int n){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->tip = strdup("int");
    e->isVar = false;
    e->isString = false;
    e->exp.left = NULL;
    e->val = (float)n;
    return e;
}

struct expr_type* create_expr(float n){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->tip = strdup("float");
    e->isVar = false;
    e->isString = false;
    e->exp.left = NULL;
    e->val = (float)n;
    return e;
}

struct expr_type* create_expr(bool n){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->tip = strdup("bool");
    e->isVar = false;
    e->isString = false;
    e->exp.left = NULL;
    e->val = (float)n;
    return e;
}

struct expr_type* create_expr(char n){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->tip = strdup("char");
    e->isVar = false;
    e->isString = false;
    e->exp.left = NULL;
    e->val = (float)n;
    return e;
}

struct expr_type* create_expr(char* id, bool isString = 0){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->isString = isString;
    if(isString){
        e->isVar = false;
        e->sVal = strdup(id);
    }
    else{
        e->isVar = true;
        e->var.id = strdup(id);
        e->var.isArr = false;
    }
    return e;
}

struct expr_type* create_expr(char* id, int dim){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->isVar = true;
    e->var.id = strdup(id);
    e->var.isArr = true;
    e->var.indexNo = dim;
    return e;
}

struct expr_type* expr(struct expr_type *l, struct expr_type *r, int op){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->isVar = false;
    e->exp.left = l;
    e->exp.right = r;
    r->exp.op = op;
    return e;
}

struct expr_type* expr_negate(struct expr_type *toNegate){
    struct expr_type *n = (struct expr_type*) malloc(sizeof(struct expr_type));
    n->isVar = false;
    n->exp.left = NULL;
    n->tip = strdup("bool"); // because we always promote the type of the expression. Therefore let's give it the "smallest" one
    n->val = -1;
    return expr(n, toNegate, 3);
}

struct expr_type* expr(struct expr_type *l, struct expr_type *r, char *op){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->isVar = false;
    e->exp.left = l;
    e->exp.right = r;
    if(strcmp(op, "==") == 0)
        e->exp.op = 7;
    else if(strcmp(op, "<") == 0)
        e->exp.op = 8;
    else if(strcmp(op, "<=") == 0)
        e->exp.op = 9;
    else if(strcmp(op, ">") == 0)
        e->exp.op = 10;
    else
        e->exp.op = 11;
    return e;
}

struct expr_type* expr_negation(struct expr_type *l){
    struct expr_type *e = (struct expr_type*) malloc(sizeof(struct expr_type));
    e->isVar = false;
    e->exp.left = l;
    e->exp.op = 12;
    return e;
}