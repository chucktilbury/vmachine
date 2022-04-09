

#include "common.h"
#include "arith_expr.h"

int do_OP_ADD(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    addVals(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_SUB(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    subVals(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_MUL(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    mulVals(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_DIV(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    divVals(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_MOD(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    modVals(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_NEG(VMachine* vm)
{
    Value* val;
    POP(vm, val);
    negVal(val);
    PUSH(vm, val);
    return 0;
}

int do_OP_CAST(VMachine* vm)
{
    uint8_t type = READ8(vm);
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);
    castToType(val, type);
    return 0;
}

