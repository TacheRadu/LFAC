struct sign{
    struct sign* next;
    char *tip;
    char *id;
    bool isConst;
};

struct sign* signEntry(char* tip, char* id, bool isConst = 0){
    struct sign *e = (struct sign*) malloc(sizeof(struct sign));
    e->next = NULL;
    e->tip = strdup(tip);
    e->id = strdup(id);
    e->isConst = isConst;
    return e;
}