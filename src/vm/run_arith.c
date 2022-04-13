

#include "common.h"
#include "arith_expr.h"

#define ABINARY(name) do { \
        StkVal left, right; \
        StkVal result; \
        left = popVal(); \
        right = popVal(); \
        name(result, right, left); \
        pushVal(result); } while(0)

int do_OP_ADD()
{
    // Value* left, *right;
    // Value* result = createVal(VAL_ERROR);
    // POP(vm, left);
    // POP(vm, right);
    // addVals(result, right, left);
    // PUSH(vm, result);
    StkVal left, right;
    StkVal result;
    left = popVal();
    right = popVal();
    result = addVals(right, left);
    pushVal(result);
    return 0;
}

int do_OP_SUB()
{
    // Value* left, *right;
    // Value* result = createVal(VAL_ERROR);
    // POP(vm, left);
    // POP(vm, right);
    // subVals(result, right, left);
    // PUSH(vm, result);
    StkVal left, right;
    StkVal result;
    left = popVal();
    right = popVal();
    result = subVals(right, left);
    pushVal(result);
    return 0;
}

int do_OP_MUL()
{
    // Value* left, *right;
    // Value* result = createVal(VAL_ERROR);
    // POP(vm, left);
    // POP(vm, right);
    // mulVals(result, right, left);
    // PUSH(vm, result);
    StkVal left, right;
    StkVal result;
    left = popVal();
    right = popVal();
    result = mulVals(right, left);
    pushVal(result);
    return 0;
}

int do_OP_DIV()
{
    // Value* left, *right;
    // Value* result = createVal(VAL_ERROR);
    // POP(vm, left);
    // POP(vm, right);
    // divVals(result, right, left);
    // PUSH(vm, result);
    StkVal left, right;
    StkVal result;
    left = popVal();
    right = popVal();
    result = divVals(right, left);
    pushVal(result);
    return 0;
}

int do_OP_MOD()
{
    // Value* left, *right;
    // Value* result = createVal(VAL_ERROR);
    // POP(vm, left);
    // POP(vm, right);
    // modVals(result, right, left);
    // PUSH(vm, result);
    StkVal left, right;
    StkVal result;
    left = popVal();
    right = popVal();
    result = modVals(right, left);
    pushVal(result);
    return 0;
}

int do_OP_NEG()
{
    StkVal val;
    val = popVal();
    val = negVal(val);
    pushVal(val);
    return 0;
}

int do_OP_CAST()
{
    fprintf(stderr, "CAST instruction not implemented\n");
    exit(1);
    // uint8_t type = READ8(vm);
    // Index idx = READ16(vm);
    // Value* val = getVal(vm->val_store, idx);
    // castToType(val, type);
    return 0;
}

