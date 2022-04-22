
#include "common.h"
#include "vm.h"


int do_OP_JMP()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);
    setIndex(var->data.addr);
    return 0;
}

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
    StkVal result = peekStk();

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

int do_OP_CALL()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);

    pushCall(initCallElem(getIndex(), getValStackLen()));

    setIndex(var->data.addr);
    return 0;
}

int do_OP_RETURN()
{
    CallElem ce = popCall();
    setIndex(ce.ret_addr);
    return 0;
}

int do_OP_CALLX()
{
    runtimeError("CALLX is not implemented");
    return 1;
}

