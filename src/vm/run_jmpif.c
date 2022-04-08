
#include "common.h"

/**
 * @brief Jump to the absolute address given by the immediate variable index
 * if the item on the top of the stack represents a boolean false.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPIF(VMachine* vm)
{
                    Index idx = READ16(vm);
                    Value* val = getVal(vm->val_store, idx);
                    Value* result;
                    POP(vm, result);
                    printVal(val);
                    switch(val->type) {
                        case VAL_ADDRESS:
                        case VAL_UNUM:
                            if(!result->data.boolean)
                                vm->inst->index = val->data.unum;
                            break;
                        case VAL_INUM:
                            if(!result->data.boolean)
                                vm->inst->index = val->data.inum;
                            break;
                        case VAL_BOOL:
                        case VAL_ERROR:
                        case VAL_FNUM:
                        case VAL_OBJ:
                            runtimeError("cannot use a %s as a jmp destination", valToStr(val->type));
                            break;
                    }

    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned char
 * if the item on the top of the stack represents a boolean false.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPIF8(VMachine* vm)
{
    int addr = (uint8_t)READ8(vm);
    Value* val;
    POP(vm, val);
    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned short
 * if the item on the top of the stack represents a boolean false.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPIF16(VMachine* vm)
{
    int addr = (uint16_t)READ16(vm);
    vm->inst->index = addr;
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned int
 * if the item on the top of the stack represents a boolean false.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPIF32(VMachine* vm)
{
    int addr = (uint32_t)READ32(vm);
    vm->inst->index = addr;
    return 0;
}

int do_OP_JMPIFR(VMachine* vm)
{
    Index idx = READ16(vm);

    return 0;
}

int do_OP_JMPIFR8(VMachine* vm)
{
    int addr = (int8_t)READ8(vm);
    vm->inst->index += addr;
    return 0;
}

int do_OP_JMPIFR16(VMachine* vm)
{
    int addr = (int16_t)READ16(vm);
    vm->inst->index += addr;
    return 0;
}

int do_OP_JMPIFR32(VMachine* vm)
{
    int addr = (int32_t)READ32(vm);
    vm->inst->index += addr;
    return 0;
}
