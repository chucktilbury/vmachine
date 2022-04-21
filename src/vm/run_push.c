
#include "common.h"
#include "vm.h"

int do_OP_PUSH()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);
    //printf("var type: %s\t", varTypeToStr(var->type));
    pushVal(convertVarToVal(var));
    return 0;
}

int do_OP_PUSH8()
{
    uint8_t type = read8();
    uint32_t valu = read8();
    //printf("type: %s, val = %u\t", varTypeToStr(type), valu);
    StkVal val = initVal(type, &valu);
    pushVal(val);
    return 0;
}

int do_OP_PUSH16()
{
    uint8_t type = read8();
    uint32_t valu = read16();
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

int do_OP_PEEK()
{
    short ofst = (short)read16();
    int base = peekCallBase();
    //printf("\noffset: %d, base: %d, effective: %d", ofst, base, ofst+base);
    StkVal val = peekVal(ofst+base);
    //printf("\nvalue: %d\t", val.data.inum);
    pushVal(val);
    return 0;
}


