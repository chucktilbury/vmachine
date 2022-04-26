

#include "common.h"
#include "vm.h"

int do_OP_ADD()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(addVals(left, right));
    return 0;
}

int do_OP_SUB()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(subVals(left, right));
    return 0;
}

int do_OP_MUL()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(mulVals(left, right));
    return 0;
}

int do_OP_DIV()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(divVals(left, right));
    return 0;
}

int do_OP_MOD()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(modVals(left, right));
    return 0;
}

int do_OP_NEG()
{
    pushVal(negVal(popVal()));
    return 0;
}


