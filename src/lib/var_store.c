
#include "common.h"
#include "var_store.h"
#include "scanner.h"

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
        for(size_t i = 0; i < store.len; i++) {
            _free(store.list[i]);
        }
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
#ifdef DEBUG_INFO
    strncpy(var->file, get_file_name(), sizeof(var->file));
    var->line = get_line_number();
    var->col = get_col_number();
#endif

    return var;
}

int addVar(Variable* var)
{
    if((store.len + 1) > store.cap) {
        store.cap <<= 1;
        store.list = _realloc_ds_array(store.list, Variable*, store.cap);
    }

    store.list[store.len] = var;
    store.len++;
    return store.len - 1;
}

Variable* getVar(int index)
{
    if(index >= 0 && index < (int)store.len) {
        return store.list[index];
    }
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

// assign the data, while preserving the type in the dest.
void assignVar(Variable* dest, Variable* src)
{
    dest->isAssigned = true;
    switch(dest->type) {
        case VAL_OBJ:
        case VAL_ERROR:
            switch(src->type) {
                case VAL_ERROR:
                case VAL_OBJ:
                    dest->data.obj = src->data.obj;
                    break;
                case VAL_NOTHING:
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
                    exit(1);
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;

        case VAL_NOTHING:
            fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
            exit(1);
            break;

        case VAL_UNUM:
            switch(src->type) {
                case VAL_ERROR:
                case VAL_OBJ:
                case VAL_NOTHING:
                    fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    dest->data.unum = src->data.unum;
                    break;
                case VAL_INUM:
                    dest->data.unum = (uint32_t)src->data.inum;
                    break;
                case VAL_FNUM:
                    dest->data.unum = (uint32_t)((int32_t)src->data.fnum);
                    break;
                case VAL_BOOL:
                    dest->data.unum = src->data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    dest->data.unum = src->data.addr;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;

        case VAL_INUM:
            switch(src->type) {
                case VAL_ERROR:
                case VAL_OBJ:
                case VAL_NOTHING:
                    fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    dest->data.inum = (int32_t)src->data.unum;
                    break;
                case VAL_INUM:
                    dest->data.inum = src->data.inum;
                    break;
                case VAL_FNUM:
                    dest->data.inum = (int32_t)src->data.fnum;
                    break;
                case VAL_BOOL:
                    dest->data.inum = src->data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    dest->data.inum = (int32_t)src->data.addr;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;

        case VAL_FNUM:
            switch(src->type) {
                case VAL_ERROR:
                case VAL_OBJ:
                case VAL_NOTHING:
                case VAL_ADDRESS:
                    fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    dest->data.fnum = (float)((int32_t)src->data.unum);
                    break;
                case VAL_INUM:
                    dest->data.fnum = (float)src->data.inum;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = src->data.fnum;
                    break;
                case VAL_BOOL:
                    dest->data.fnum = src->data.boolean ? 1 : 0;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;

        case VAL_BOOL:
            switch(src->type) {
                // todo: objects have their own way to give a boolean
                case VAL_OBJ:
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_ADDRESS:
                    dest->data.boolean = true;
                    break;
                case VAL_UNUM:
                    dest->data.boolean = src->data.unum ? true : false;
                    break;
                case VAL_INUM:
                    dest->data.boolean = src->data.inum ? true : false;
                    break;
                case VAL_FNUM:
                    dest->data.boolean = src->data.fnum != 0.0 ? true : false;
                    break;
                case VAL_BOOL:
                    dest->data.boolean = src->data.boolean;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;
        case VAL_ADDRESS:
            switch(src->type) {
                case VAL_ERROR:
                case VAL_OBJ:
                case VAL_NOTHING:
                case VAL_FNUM:
                case VAL_BOOL:
                    fprintf(stderr, "ERROR: cannot assign a %s to a %s\n", varTypeToStr(src->type), varTypeToStr(dest->type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    dest->data.addr = src->data.unum;
                    break;
                case VAL_INUM:
                    dest->data.addr = (uint32_t)src->data.inum;
                    break;
                case VAL_ADDRESS:
                    dest->data.addr = src->data.addr;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
                    exit(1);
            }
            break;

        default:
            fprintf(stderr, "ERROR: cannot assign unknown value type: %u\n", dest->type);
            exit(1);
    }

}

// change the type of this var, keeping the data straight.
// todo: this gets complicated with objects. Objects are ignored for now.
void castVar(Variable* var, uint8_t type)
{
    switch(type) {
        case VAL_ERROR:
            switch(var->type) {
                case VAL_NOTHING:
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_ERROR:
                case VAL_OBJ:
                    // no change required
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_NOTHING:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_OBJ:
                case VAL_ADDRESS:
                    var->type = type;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_UNUM:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_OBJ:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    // no change needed
                    break;
                case VAL_INUM:
                    //Variable* tmp = createVar(type);
                    var->type = type;
                    var->data.unum = (uint32_t)var->data.inum;
                    break;
                case VAL_FNUM:
                    var->type = type;
                    var->data.unum = (uint32_t)((int32_t)var->data.fnum);
                    break;
                case VAL_BOOL:
                    var->type = type;
                    var->data.unum = var->data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    var->type = type;
                    var->data.unum = var->data.addr;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_INUM:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_OBJ:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    var->type = type;
                    var->data.inum = (int32_t)var->data.unum;
                    break;
                case VAL_INUM:
                    // no change
                    break;
                case VAL_FNUM:
                    var->type = type;
                    var->data.inum = (int32_t)var->data.fnum;
                    break;
                case VAL_BOOL:
                    var->type = type;
                    var->data.inum = var->data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    var->type = type;
                    var->data.inum = (int32_t)var->data.addr;
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_FNUM:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_OBJ:
                case VAL_ADDRESS:
                case VAL_BOOL:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_UNUM:
                    var->type = type;
                    var->data.fnum = (float)((int32_t)var->data.unum);
                    break;
                case VAL_INUM:
                    var->type = type;
                    var->data.fnum = (float)var->data.inum;
                    break;
                case VAL_FNUM:
                    // no change
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_BOOL:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_OBJ:
                case VAL_ADDRESS:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_UNUM:
                case VAL_INUM:
                    var->type = type;
                    var->data.boolean = var->data.unum ? true : false;
                    break;
                case VAL_FNUM:
                    var->type = type;
                    var->data.boolean = var->data.fnum != 0.0 ? true : false;
                    break;
                case VAL_BOOL:
                    // no change
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_OBJ:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_OBJ:
                    // no change
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        case VAL_ADDRESS:
            switch(var->type) {
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_OBJ:
                    fprintf(stderr, "ERROR: cannot cast a %s to a %s\n", varTypeToStr(var->type), varTypeToStr(type));
                    exit(1);
                    break;
                case VAL_INUM:
                    var->type = type;
                    var->data.addr = (uint32_t)var->data.inum;
                    break;
                case VAL_UNUM:
                case VAL_ADDRESS:
                    // no change
                    break;
                default:
                    fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", var->type);
                    exit(1);
            }
            break;

        default:
            fprintf(stderr, "ERROR: cannot cast unknown type: %d\n", type);
            exit(1);
    }
}

void printVar(Variable* var)
{
    printf("%-12s", varTypeToStr(var->type));
    switch(var->type) {
        case VAL_ERROR:
            printf("ERROR");
            break;
        case VAL_OBJ:
            printf("%p", var->data.obj);
            break;
        case VAL_UNUM:
            printf("0x%X", var->data.unum);
            break;
        case VAL_ADDRESS:
            printf("%d", var->data.addr);
            break;
        case VAL_INUM:
            printf("%d", var->data.inum);
            break;
        case VAL_FNUM:
            printf("%0.1f", var->data.fnum);
            break;
        case VAL_BOOL:
            printf("%s", var->data.boolean ? "TRUE" : "FALSE");
            break;
        default:
            printf("object value not found");
            break;
    }

    printf("\tassigned: %-5s const: %-5s literal: %-5s ",
           var->isAssigned ? "true" : "false",
           var->isConst ? "true" : "false",
           var->isLiteral ? "true" : "false");
    //printf("hash: 0x%08X", var->hash);

    // printf("raw data: 0x%08X\n", var->data.unum);

    //printf("\n");
}

const char* varTypeToStr(int type)
{
    return (type == VAL_ERROR) ? "ERROR" :
           (type == VAL_NOTHING) ? "NOTHING" :
           (type == VAL_UNUM) ? "UNUM" :
           (type == VAL_INUM) ? "INUM" :
           (type == VAL_FNUM) ? "FNUM" :
           (type == VAL_BOOL) ? "BOOL" :
           (type == VAL_OBJ) ? "OBJ" :
           (type == VAL_ADDRESS) ? "ADDRESS" : "UNKNOWN";
}

void saveVarStore(FILE* fp)
{
    // TODO: does not handle objects.
    fwrite(&store.len, sizeof(size_t), 1, fp);
    for(size_t i = 0; i < store.len; i++) {
        fwrite(store.list[i], sizeof(Variable), 1, fp);
    }
}

void loadVarStore(FILE* fp)
{
    // TODO: does not handle objects
    memset(&store, 0, sizeof(VarStore));
    fread(&store.len, sizeof(size_t), 1, fp);

    store.cap = 1;
    while((store.len + 1) > store.cap) {
        store.cap <<= 1;
    }

    store.list = _alloc_ds_array(Variable*, store.cap);

    Variable* var;
    for(size_t i = 0; i < store.len; i++) {
        var = _alloc_ds(Variable);
        fread(var, sizeof(Variable), 1, fp);
        store.list[i] = var;
    }
    //printf("len: %lu cap: %lu list: %p\n", store.len, store.cap, store.list);

}

