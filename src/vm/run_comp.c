
#include "common.h"
#include "comp_expr.h"

int do_OP_NOT(VMachine* vm)
{
    Value* val, *result = createVal(VAL_ERROR);
    POP(vm, val);
    notVal(result, val);
    PUSH(vm, result);
    return 0;
}

int do_OP_EQ(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    eqVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_NEQ(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    neqVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_LEQ(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    leqVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_GEQ(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    geqVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_LESS(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    lessVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

int do_OP_GTR(VMachine* vm)
{
    Value* left, *right;
    Value* result = createVal(VAL_ERROR);
    POP(vm, left);
    POP(vm, right);
    gtrVal(result, right, left);
    PUSH(vm, result);
    return 0;
}

