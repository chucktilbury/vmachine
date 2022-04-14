
#include "common.h"
#include "vmachine.h"



void createVMachine()
{
    createInstStore();
    createVarStore();
    createValStack();
    createCallStack();
}

void destroyVMachine()
{
    destroyInstStore();
    destroyVarStore();
    destroyValStack();
    destroyCallStack();
}

Variable* convertValToVar(StkVal val)
{
    Variable* var = createVar(val.type);
    switch(val.type) {
        case VAL_OBJ:
        case VAL_ERROR: var->data.obj = val.data.obj; break;
        case VAL_NOTHING: var->data.obj = NULL; break;
        case VAL_UNUM: var->data.unum = val.data.unum; break;
        case VAL_INUM: var->data.inum = val.data.inum; break;
        case VAL_FNUM: var->data.fnum = val.data.fnum; break;
        case VAL_BOOL: var->data.boolean = val.data.boolean; break;
        case VAL_ADDRESS: var->data.addr = val.data.addr; break;
        default:
            fprintf(stderr, "Fatal Error: cannot convert value type 0x%02X\n", val.type);
            exit(1);

    }

    return var;
}

StkVal convertVarToVal(Variable* var)
{
    StkVal val;
    switch(var->type) {
        case VAL_OBJ:
        case VAL_ERROR: val.data.obj = var->data.obj; break;
        case VAL_NOTHING: val.data.obj = NULL; break;
        case VAL_UNUM: val.data.unum = var->data.unum; break;
        case VAL_INUM: val.data.inum = var->data.inum; break;
        case VAL_FNUM: val.data.fnum = var->data.fnum; break;
        case VAL_BOOL: val.data.boolean = var->data.boolean; break;
        case VAL_ADDRESS: val.data.addr = var->data.addr; break;
        default:
            fprintf(stderr, "Fatal Error: cannot convert variable type 0x%02X\n", var->type);
            exit(1);
    }
    val.type = var->type;

    return val;
}
