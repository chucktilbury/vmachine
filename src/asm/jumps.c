
#include "common.h"

// defined in main.c
void syntaxError(const char* fmt, ...);

static int get_bits(Value *val)
{
    int retv = 0;
    uint32_t n = val->data.unum; // get the raw bits

    if(val->type == VAL_FNUM)
        retv = 32;
    else {
        int count = 0;
        for(int i = 0; i < 32; i++) {
            if(n & (0x01 << i))
                count++;
        }

        if(count >= 0 && count < 7)
            retv = 8;
        else if(count >= 8 && count < 15)
            retv = 16;
        else
            retv = 32;
    }

    return retv;
}

void emitJMP(VMachine* vm, Value* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("jump instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_JMP8);
            WRITE8(vm, val->data.unum);
            break;
        case 16:
            WRITE8(vm, OP_JMP16);
            WRITE16(vm, val->data.unum);
            break;
        case 32:
            WRITE8(vm, OP_JMP32);
            WRITE32(vm, val->data.unum);
            break;
    }
}

void emitJMPIF(VMachine* vm, Value* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("jump instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_JMPIF8);
            WRITE8(vm, val->data.unum);
            break;
        case 16:
            WRITE8(vm, OP_JMPIF16);
            WRITE16(vm, val->data.unum);
            break;
        case 32:
            WRITE8(vm, OP_JMPIF32);
            WRITE32(vm, val->data.unum);
            break;
    }
}

void emitCALL(VMachine* vm, Value* val)
{
    if(val->type != VAL_ADDRESS && val->type != VAL_UNUM) {
        syntaxError("call instruction requires an address or an unsigned number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_CALL8);
            WRITE8(vm, val->data.unum);
            break;
        case 16:
            WRITE8(vm, OP_CALL16);
            WRITE16(vm, val->data.unum);
            break;
        case 32:
            WRITE8(vm, OP_CALL32);
            WRITE32(vm, val->data.unum);
            break;
    }
}

void emitJMPR(VMachine* vm, Value* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("jump relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_JMPR8);
            WRITE8(vm, val->data.inum);
            break;
        case 16:
            WRITE8(vm, OP_JMPR16);
            WRITE16(vm, val->data.inum);
            break;
        case 32:
            WRITE8(vm, OP_JMPR32);
            WRITE32(vm, val->data.inum);
            break;
    }
}

void emitJMPIFR(VMachine* vm, Value* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("jmpif relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_JMPIFR8);
            WRITE8(vm, val->data.inum);
            break;
        case 16:
            WRITE8(vm, OP_JMPIFR16);
            WRITE16(vm, val->data.inum);
            break;
        case 32:
            WRITE8(vm, OP_JMPIFR32);
            WRITE32(vm, val->data.inum);
            break;
    }
}

void emitCALLR(VMachine* vm, Value* val)
{
    if(val->type != VAL_INUM) {
        syntaxError("call relative instruction requires a signed number");
        return;
    }

    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_CALLR8);
            WRITE8(vm, val->data.inum);
            break;
        case 16:
            WRITE8(vm, OP_CALLR16);
            WRITE16(vm, val->data.inum);
            break;
        case 32:
            WRITE8(vm, OP_CALLR32);
            WRITE32(vm, val->data.inum);
            break;
    }
}

void emitPUSH(VMachine* vm, Value* val)
{
    switch(get_bits(val)) {
        case 8:
            WRITE8(vm, OP_PUSH8);
            WRITE8(vm, val->type);
            WRITE8(vm, val->data.unum);
            break;
        case 16:
            WRITE8(vm, OP_PUSH16);
            WRITE8(vm, val->type);
            WRITE16(vm, val->data.unum);
            break;
        case 32:
            WRITE8(vm, OP_PUSH32);
            WRITE8(vm, val->type);
            WRITE32(vm, val->data.unum);
            break;
    }
}
