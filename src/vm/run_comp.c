
#include "common.h"
#include "vm.h"

int do_OP_NOT()
{
    pushVal(notVal(popVal()));
    return 0;
}

int do_OP_EQ()
{
    pushVal(eqVal(popVal(), popVal()));
    return 0;
}

int do_OP_NEQ()
{
    pushVal(neqVal(popVal(), popVal()));
    return 0;
}

int do_OP_LEQ()
{
    pushVal(leqVal(popVal(), popVal()));
    return 0;
}

int do_OP_GEQ()
{
    pushVal(geqVal(popVal(), popVal()));
    return 0;
}

int do_OP_LESS()
{
    pushVal(lessVal(popVal(), popVal()));
    return 0;
}

int do_OP_GTR()
{
    pushVal(gtrVal(popVal(), popVal()));
    return 0;
}

