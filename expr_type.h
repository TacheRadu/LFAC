#define max(X, Y) (((X) < (Y)) ? (X) : (Y))

extern int yylineno;

struct expr_type{
    int tip;
    // int: 0
    // char: 1
    // bool: 2
    // float: 3
    
    float val;
};

struct expr_type* create_expr(float n){
    struct expr_type *res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = 3;
    res->val = n;
    return res;
}

struct expr_type* create_expr(int n){
    struct expr_type *res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = 0;
    res->val = (float)n;
    return res;
}

struct expr_type* create_expr(char c){
    struct expr_type *res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = 1;
    res->val = (float)c;
    return res;
}

struct expr_type* create_expr(int b, int smth){
    struct expr_type *res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = 2;
    res->val = (float)b;
    return res;
}

struct expr_type* logical_operations(struct expr_type *left, struct expr_type *right, char *op){
    struct expr_type *res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = 2;
    if(strcmp(op, "==") == 0)
        res->val = (float)(left->val == right->val);
    else if(strcmp(op, "<") == 0)
        res->val = (float)(left->val < right->val);
    else if(strcmp(op, "<=") == 0)
        res->val = (float)(left->val <= right->val);
    else if(strcmp(op, ">") == 0)
        res->val = (float)(left->val > right->val);
    else if(strcmp(op, ">=") == 0)
        res->val = (float)(left->val >= right->val);
    return res;
}

struct expr_type* expr_aritm(struct expr_type *a, struct expr_type *b, char op){
    struct expr_type* res = (struct expr_type*) malloc(sizeof(struct expr_type));
    res->tip = max(a->tip, b->tip);
    switch(op){
        case '+':
            res->val = a->val + b->val;
            break;
        case '-':
            res->val = a->val - b->val;
            break;
        case '*':
            res->val = a->val * b->val;
            break;
        case '/':
            res->val = a->val / b->val;
    }
    return res;
}