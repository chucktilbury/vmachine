
#include "common.h"

int do_OP_PUSH()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);
    pushVal(convertVarToVal(var));
    return 0;
}

int do_OP_PUSH8()
{
    uint8_t type = read8();
    uint8_t valu = read8();
    StkVal val = initVal(type, &valu);
    pushVal(val);
    return 0;
}

int do_OP_PUSH16()
{
    uint8_t type = read8();
    uint16_t valu = read16();
    StkVal val = initVal(type, &valu);
    pushVal(val);
    return 0;
}

int do_OP_PUSH32()
{
    uint8_t type = read8();
    uint32_t valu = read32();
    StkVal val = initVal(type, &valu);
    pushVal(val);
    return 0;
}

int do_OP_POP()
{
    popVal();
    return 0;
}

