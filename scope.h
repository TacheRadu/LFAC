
struct scope{
    struct scope_entry *first_item;
};

struct scope* scopeFromEntry(struct scope_entry* e){
    struct scope* s = (struct scope*) malloc(sizeof(struct scope));
    s->first_item = e;
    return s;
}
