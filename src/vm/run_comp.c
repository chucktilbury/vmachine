
#include "common.h"
#include "vm.h"

int do_OP_NOT()
{
    pushVal(notVal(popVal()));
    return 0;
}

int do_OP_EQ()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(eqVal(left, right));

    return 0;
}

int do_OP_NEQ()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(neqVal(left, right));
    return 0;
}

int do_OP_LEQ()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(leqVal(left, right));
    return 0;
}

int do_OP_GEQ()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(geqVal(left, right));
    return 0;
}

int do_OP_LESS()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(lessVal(left, right));
    return 0;
}

int do_OP_GTR()
{
    StkVal right = popVal();
    StkVal left = popVal();
    pushVal(gtrVal(left, right));
    return 0;
}

