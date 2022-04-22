
#include "common.h"
#include "asm.h"

// defined in main.c
void syntaxError(const char* fmt, ...);

// Notes: If the value is a constant, then the relative instruction can be
// used. This is useful because a short immediate relative instruction is
// more efficient than looking up a variable value. Note that labels are
// automatically marked as constant literals. Compare the val value to the
// current value of the IP and decide what to do.

static int get_bits(Variable* val)
{
    int retv = 0;
    uint32_t n = val->data.unum; // get the raw bits

    if(val->type == VAL_FNUM) {
        retv = 32;
    }
    else {
        int count = 0;
        for(int i = 0; i < 32; i++) {
            if(n & (0x01 << i)) {
                count = i;
            }
        }

        if(count >= 0 && count < 8) {
            retv = 8;
        }
        else if(count >= 8 && count < 16) {
            retv = 16;
        }
        else {
            printf(">>> Count = %d\n", count);
            retv = 32;
        }
    }

    return retv;
}

void emitJMP(Variable* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("jump instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_JMP8);
            write8(val->data.unum);
            break;
        case 16:
            write8(OP_JMP16);
            write16(val->data.unum);
            break;
        case 32:
            write8(OP_JMP32);
            write32(val->data.unum);
            break;
    }
}

void emitJMPIF(Variable* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("jump instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_JMPIF8);
            write8(val->data.unum);
            break;
        case 16:
            write8(OP_JMPIF16);
            write16(val->data.unum);
            break;
        case 32:
            write8(OP_JMPIF32);
            write32(val->data.unum);
            break;
    }
}

void emitCALL(Variable* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("call instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_CALL8);
            write8(val->data.unum);
            break;
        case 16:
            write8(OP_CALL16);
            write16(val->data.unum);
            break;
        case 32:
            write8(OP_CALL32);
            write32(val->data.unum);
            break;
    }
}

void emitJMPR(Variable* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("jump relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_JMPR8);
            write8(val->data.inum);
            break;
        case 16:
            write8(OP_JMPR16);
            write16(val->data.inum);
            break;
        case 32:
            write8(OP_JMPR32);
            write32(val->data.inum);
            break;
    }
}

void emitJMPIFR(Variable* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("jmpif relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_JMPIFR8);
            write8(val->data.inum);
            break;
        case 16:
            write8(OP_JMPIFR16);
            write16(val->data.inum);
            break;
        case 32:
            write8(OP_JMPIFR32);
            write32(val->data.inum);
            break;
    }
}

void emitCALLR(Variable* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("call relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            write8(OP_CALLR8);
            write8(val->data.inum);
            break;
        case 16:
            write8(OP_CALLR16);
            write16(val->data.inum);
            break;
        case 32:
            write8(OP_CALLR32);
            write32(val->data.inum);
            break;
    }
}

void emitPUSH(Variable* val)
{
    switch(get_bits(val)) {
        case 8:
            write8(OP_PUSH8);
            write8(val->type);
            write8(val->data.unum);
            //printf("type: %s, value: %u\n", varTypeToStr(val->type), val->data.unum);
            break;
        case 16:
            write8(OP_PUSH16);
            write8(val->type);
            write16(val->data.unum);
            break;
        case 32:
            write8(OP_PUSH32);
            write8(val->type);
            write32(val->data.unum);
            break;
    }
}

void emitPeek(Variable* val)
{
    write8(OP_PEEK);
    switch(val->type) {
        case VAL_ERROR:
        case VAL_NOTHING:
        case VAL_UNUM:
        case VAL_FNUM:
        case VAL_BOOL:
        case VAL_OBJ:
        case VAL_ADDRESS:
            syntaxError("peek instruction expects a INUM, not a %s", varTypeToStr(val->type));
            return;

        case VAL_INUM:
            write16((int16_t)val->data.inum);
            break;

        default:
            fatalError("unknown variable type: %d", val->type);
    }
}

void emitLocal(Variable* val)
{
    write8(OP_LOCAL);
    switch(val->type) {
        case VAL_ERROR:
        case VAL_NOTHING:
        case VAL_UNUM:
        case VAL_FNUM:
        case VAL_BOOL:
        case VAL_OBJ:
        case VAL_ADDRESS:
            syntaxError("local instruction expects a INUM, not a %s", varTypeToStr(val->type));
            return;

        case VAL_INUM:
            write16((int16_t)val->data.inum);
            break;

        default:
            fatalError("unknown variable type: %d", val->type);
    }
}
