#include "scope_entry.h"

struct scope{
    struct scope *prev;
    struct scope_entry *first_item;
};

struct scope* scopeFromEntry(struct scope_entry* e){
    struct scope* s = (struct scope*) malloc(sizeof(struct scope));
    s->first_item = e;
    s->first_item->next = NULL;
    s->first_item->prev = NULL;
    return s;
}
