struct expr_string{
    char* tip;
    bool isVar;
    union{
        struct {
            bool isArr;
            int indexNo;
            char *id;
            struct scope_entry *var;
        }var;
        struct {
            struct expr_type *left, *right;
        }exp;
    };
    char* sVal;
    bool bVal;
};