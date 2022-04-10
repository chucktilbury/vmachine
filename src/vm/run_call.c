
#include "common.h"

int do_OP_CALLX(VMachine* vm)
{
    (void)vm;
    runtimeError("CALLX is not implemented");
    return 1;
}

/**
 * @brief Call the absolute address contained in the variable which is given
 * by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL(VMachine* vm)
{
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index = val->data.unum;
    return 0;
}

/**
 * @brief Call the absolute address given by the immediate unsigned char value.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL8(VMachine* vm)
{
    int addr = (uint8_t)READ8(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Call the absolute address given by the immediate unsigned short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL16(VMachine* vm)
{
    int addr = (uint16_t)READ16(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Call the absolute address given by the immediate unsigned int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALL32(VMachine* vm)
{
    int addr = (uint32_t)READ32(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index = addr;
    return 0;
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
int do_OP_CALLR(VMachine* vm)
{
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index += val->data.unum;
    return 0;
}

/**
 * @brief Call the relative address given by the immediate signed char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR8(VMachine* vm)
{
    int addr = (int8_t)READ8(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index += addr;
    return 0;
}

/**
 * @brief Call the relative address given by the immediate signed short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR16(VMachine* vm)
{
    int addr = (int16_t)READ16(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index += addr;
    return 0;
}

/**
 * @brief Call the relative address given by the immediate signed int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_CALLR32(VMachine* vm)
{
    int addr = (int32_t)READ32(vm);

    Value* ret = createVal(VAL_ADDRESS);
    ret->data.unum = vm->inst->index;
    PUSH(vm, ret);

    vm->inst->index += addr;
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
int do_OP_RETURN(VMachine* vm)
{
    Value* val;
    POP(vm, val);
    vm->inst->index = val->data.unum;
    return 0;
}

