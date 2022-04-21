

#include "common.h"
#include "vm.h"

#define ABINARY(name) do { \
        StkVal left, right; \
        StkVal result; \
        left = popVal(); \
        right = popVal(); \
        name(result, right, left); \
        pushVal(result); } while(0)

int do_OP_ADD()
{
    pushVal(addVals(popVal(), popVal()));
    return 0;
}

int do_OP_SUB()
{
    pushVal(subVals(popVal(), popVal()));
    return 0;
}

int do_OP_MUL()
{
    pushVal(mulVals(popVal(), popVal()));
    return 0;
}

int do_OP_DIV()
{
    pushVal(divVals(popVal(), popVal()));
    return 0;
}

int do_OP_MOD()
{
    pushVal(modVals(popVal(), popVal()));
    return 0;
}

int do_OP_NEG()
{
    pushVal(negVal(popVal()));
    return 0;
}


