
#include "common.h"

/**
 * @brief Jump to the absolute address given by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP(VMachine* vm)
{
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);
    vm->inst->index = val->data.unum;
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP8(VMachine* vm)
{
    int addr = (uint8_t)READ8(vm);
    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP16(VMachine* vm)
{
    int addr = (uint16_t)READ16(vm);
    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP32(VMachine* vm)
{
    int addr = (uint32_t)READ32(vm);
    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Jump to the relative address given by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR(VMachine* vm)
{
    Index idx = READ16(vm);
    //vm->inst->index += addr;
    return 0;
}

/**
 * @brief Jump the relative address given by the immediate signed char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR8(VMachine* vm)
{
    int addr = (int8_t)READ8(vm);
    vm->inst->index += addr;
    return 0;
}

/**
 * @brief Jump the relative address given by the immediate signed short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR16(VMachine* vm)
{
    int addr = (int16_t)READ16(vm);
    vm->inst->index += addr;
    return 0;
}

/**
 * @brief Jump the relative address given by the immediate signed int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR32(VMachine* vm)
{
    int addr = (int32_t)READ32(vm);
    vm->inst->index += addr;
    return 0;
}
