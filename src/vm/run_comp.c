
#include "common.h"
#include "comp_expr.h"

int do_OP_NOT()
{
    StkVal val, result;
    popVal(val);
    result = notVal(val);
    pushVal(result);
    return 0;
}

int do_OP_EQ()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = eqVal(right, left);
    pushVal(result);
    return 0;
}

int do_OP_NEQ()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = neqVal(right, left);
    pushVal(result);
    return 0;
}

int do_OP_LEQ()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = leqVal(right, left);
    pushVal(result);
    return 0;
}

int do_OP_GEQ()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = geqVal(right, left);
    pushVal(result);
    return 0;
}

int do_OP_LESS()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = lessVal(right, left);
    pushVal(result);
    return 0;
}

int do_OP_GTR()
{
    StkVal left, right;
    StkVal result;
    popVal(left);
    popVal(right);
    result = gtrVal(right, left);
    pushVal(result);
    return 0;
}

