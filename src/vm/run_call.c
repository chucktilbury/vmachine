
#include "common.h"
#include "var_store.h"
#include "vm.h"

/**
 * @brief Call the absolute address contained in the variable which is given
 * by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);

    // Value* ret = createVal(VAL_ADDRESS);
    // ret->data.unum = vm->inst->index;
    // PUSH(vm, ret);
    pushCall(initCallElem(getIndex(), getValStackLen()));

    setIndex(var->data.addr);
    return 0;
}

/**
 * @brief Return from a call instruction. The top of the stack should be the
 * absolute return address of the call.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_RETURN()
{
    // Value* val;
    // POP(vm, val);
    // vm->inst->index = val->data.unum;
    CallElem ce = popCall();
    setIndex(ce.ret_addr);
    return 0;
}

/**
 * @brief Call the absolute address given by the immediate unsigned char value.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL8()
{
    runtimeError("CALL8 is not implemented");
    return 1;
}

/**
 * @brief Call the absolute address given by the immediate unsigned short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL16()
{
    runtimeError("CALL16 is not implemented");
    return 1;
}

/**
 * @brief Call the absolute address given by the immediate unsigned int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL32()
{
    runtimeError("CALL32 is not implemented");
    return 1;
}

/**
 * @brief Call the relative address given by the immediate variable index.
 * The value of the variable is a signed offset to the current instruction
 * pointer.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR()
{
    runtimeError("CALLR is not implemented");
    return 1;
}

/**
 * @brief Call the relative address given by the immediate signed char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR8()
{
    runtimeError("CALLR8 is not implemented");
    return 1;
}

/**
 * @brief Call the relative address given by the immediate signed short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR16()
{
    runtimeError("CALLR16 is not implemented");
    return 1;
}

/**
 * @brief Call the relative address given by the immediate signed int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR32()
{
    runtimeError("CALLR32 is not implemented");
    return 1;
}

int do_OP_CALLX()
{
    runtimeError("CALLX is not implemented");
    return 1;
}

