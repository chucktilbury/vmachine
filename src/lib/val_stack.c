
#include "common.h"
#include "val_stack.h"
#include "memory.h"
//#include "var_store.h"

static ValStack stack;

void createValStack()
{
    stack.cap = 0x01 << 8;
    stack.len = 0;
    stack.list = _alloc_ds_array(StkVal, stack.cap);
}

void destroyValStack()
{
    if(stack.list != NULL)
        _free(stack.list);
}

int getValStackLen()
{
    return stack.len;
}

StkVal initVal(uint16_t type, void* val)
{
    StkVal value;

    switch(type) {
        case VAL_OBJ:
        case VAL_ERROR: value.data.obj = val; break;
        case VAL_NOTHING: value.data.obj = NULL; break;
        case VAL_UNUM: value.data.unum = *(uint32_t*)val; break;
        case VAL_INUM: value.data.inum = *(int32_t*)val; break;
        case VAL_FNUM: value.data.fnum = *(float*)val; break;
        case VAL_BOOL: value.data.inum = *(uint8_t*)val; break;
        case VAL_ADDRESS: value.data.inum = *(uint32_t*)val; break;
        default: fprintf(stderr, "Fatal Error: invalid value type in initVal(): 0x%02X\n", type); exit(1);
    }
    value.type = type;

    return value;
}

void pushVal(StkVal val)
{
    if((stack.len+1) > stack.cap) {
        stack.cap <<= 1;
        stack.list = _realloc_ds_array(stack.list, StkVal, stack.cap);
    }

    stack.list[stack.len] = val;
    stack.len++;
}

StkVal popVal()
{
    // unsigned compare...
    if(stack.len-1 > stack.len) {
        fprintf(stderr, "pop value stack under run\n");
        exit(1);
    }

    stack.len--;
    return stack.list[stack.len];
}

StkVal peekVal()
{
    if(stack.len-1 > stack.len) {
        fprintf(stderr, "peek value stack under run\n");
        exit(1);
    }

    return stack.list[stack.len-1];
}

// Variable* valToVar(StkVal val)
// {
//     Variable* var = createVar(val.type);
//
//     var->isAssigned = true;
//     switch(val.type) {
//         case VAL_OBJ:
//         case VAL_ERROR: var->data.obj = val.data.obj; break;
//         case VAL_NOTHING:  var->data.obj = NULL; break;
//         case VAL_UNUM: var->data.unum = val.data.unum; break;
//         case VAL_INUM: var->data.inum = val.data.inum; break;
//         case VAL_FNUM: var->data.fnum = val.data.fnum; break;
//         case VAL_BOOL: var->data.boolean = val.data.boolean; break;
//         case VAL_ADDRESS: var->data.addr = val.data.addr; break;
//     }
//
//     return var;
// }
//
// StkVal varToVal(Variable* var)
// {
//     return initVal(var->type, &var->data);
// }