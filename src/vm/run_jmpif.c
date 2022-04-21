
#include "common.h"
#include "vm.h"


/**
 * @brief Jump to the absolute address given by the immediate variable index
 * if the item on the top of the stack represents a boolean false.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPIF()
{
    // get the value that holds the address
    uint16_t idx = read16();
    Variable* var = getVar(idx);

    // get the arithmetic result
    StkVal result = popVal();

    if(isFalse(result)) {
        switch(var->type) {
            case VAL_INUM:
                setIndex(var->data.inum);
                break;
            case VAL_ADDRESS:
            case VAL_UNUM:
                setIndex(var->data.unum);
                break;
            case VAL_FNUM:
            case VAL_ERROR:
            case VAL_BOOL:
            case VAL_OBJ:
            default:
                runtimeError("type %s cannot hold an address", varTypeToStr(var->type));
                return 1;
        }
    }

    return 0;
}

int do_OP_JMPIF8()
{
    runtimeError("JMPIF8 is not implemented");
    return 1;
}

int do_OP_JMPIF16()
{
    runtimeError("JMPIF16 is not implemented");
    return 1;
}

int do_OP_JMPIF32()
{
    runtimeError("JMPIF32 is not implemented");
    return 1;
}

int do_OP_JMPIFR()
{
    runtimeError("JMPIFR is not implemented");
    return 1;
}

int do_OP_JMPIFR8()
{
    runtimeError("JMPIFR8 is not implemented");
    return 1;
}

int do_OP_JMPIFR16()
{
    runtimeError("JMPIFR16 is not implemented");
    return 1;
}

int do_OP_JMPIFR32()
{
    runtimeError("JMPIFR32 is not implemented");
    return 1;
}
