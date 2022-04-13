
#include "common.h"
#include "var_store.h"

static VarStore store;

void createVarStore()
{
    store.cap = 0x01 << 8;
    store.len = 0;
    store.list = _alloc_ds_array(Variable*, store.cap);

    Variable* var = createVar(VAL_ERROR);
    addVar(var);
}

void destroyVarStore()
{
    if(store.list != NULL) {
        _free(store.list);
    }
}

Variable* createVar(uint16_t type)
{
    Variable* var = _alloc_ds(Variable);

    var->type = type;
    var->isAssigned = false;// has a value
    var->isConst = false;   // declared as constant
    var->isLiteral = false; // not connected to a symbol

    return var;
}

int addVar(Variable* var)
{
    if((store.len+1) > store.cap) {
        store.cap <<= 1;
        store.list = _realloc_ds_array(store.list, Variable*, store.cap);
    }

    store.list[store.len] = var;
    store.len++;
    return store.len - 1;
}

Variable* getVar(int index)
{
    if(index >= 0 && index < (int)store.len)
        return store.list[index];
    else {
        printf("index: %d store: %lu\n", index, store.len);
        fprintf(stderr, "what the hell stupid kind of index is that?");
        exit(1);
    }
}

Variable* copyVar(Variable* var)
{
    Variable* nvar = createVar(var->type);
    memcpy(nvar, var, sizeof(Variable));

    return nvar;
}

void printVar(Variable* var)
{
    printf("%-12s", varTypeToStr(var->type));
    switch(var->type) {
        case VAL_ERROR: printf("ERROR"); break;
        case VAL_OBJ: printf("%p", var->data.obj); break;
        case VAL_UNUM: printf("0x%X", var->data.unum); break;
        case VAL_ADDRESS: printf("%d", var->data.addr); break;
        case VAL_INUM: printf("%d", var->data.inum); break;
        case VAL_FNUM: printf("%0.1f", var->data.fnum); break;
        case VAL_BOOL: printf("%s", var->data.boolean ? "TRUE" : "FALSE"); break;
        default: printf("object value not found"); break;
    }

    printf("assigned: %-5s const: %-5s literal: %-5s ",
           var->isAssigned ? "true" : "false",
           var->isConst ? "true" : "false",
           var->isLiteral ? "true" : "false");
    //printf("hash: 0x%08X", var->hash);

    // printf("raw data: 0x%08X\n", var->data.unum);
}

const char* varTypeToStr(int type)
{
    return (type == VAL_ERROR)? "ERROR" :
        (type == VAL_NOTHING)? "NOTHING" :
        (type == VAL_UNUM)? "UNUM" :
        (type == VAL_INUM)? "INUM" :
        (type == VAL_FNUM)? "FNUM" :
        (type == VAL_BOOL)? "BOOL" :
        (type == VAL_OBJ)? "OBJ" :
        (type == VAL_ADDRESS)? "ADDRESS" : "UNKNOWN";
}

void saveVarStore(FILE* fp)
{
    // TODO: does not handle objects.
    fwrite(&store.len, sizeof(size_t), 1, fp);
    for(size_t i = 0; i < store.len; i++)
        fwrite(store.list[i], sizeof(Variable), 1, fp);
}

void loadVarStore(FILE* fp)
{
    // TODO: does not handle objects
    memset(&store, 0, sizeof(VarStore));
    fread(&store.len, sizeof(size_t), 1, fp);

    store.cap = 1;
    while((store.len+1) > store.cap)
        store.cap <<= 1;

    store.list = _alloc_ds_array(Variable*, store.cap);

    Variable* var;
    for(size_t i = 0; i < store.len; i++) {
        var = _alloc_ds(Variable);
        fread(var, sizeof(Variable), 1, fp);
        store.list[i] = var;
    }
    //printf("len: %lu cap: %lu list: %p\n", store.len, store.cap, store.list);

}

