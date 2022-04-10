
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
    // get the value that holds the address
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);

    // get the arithmetic result
    Value* result;
    POP(vm, result);

    if(isFalse(result)) {
        switch(val->type) {
            case VAL_INUM:
                vm->inst->index = val->data.inum;
                break;
            case VAL_ADDRESS:
            case VAL_UNUM:
                vm->inst->index = val->data.unum;
                break;
            case VAL_FNUM:
            case VAL_ERROR:
            case VAL_BOOL:
            case VAL_OBJ:
            default:
                runtimeError("type %s cannot hold an address", valToStr(val->type));
                return 1;
        }
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

    if(isFalse(val)) {
        vm->inst->index = addr;
    }

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
    Value* val;
    POP(vm, val);

    if(isFalse(val)) {
        vm->inst->index = addr;
    }

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
    Value* val;
    POP(vm, val);

    if(isFalse(val)) {
        vm->inst->index = addr;
    }

    return 0;
}

int do_OP_JMPIFR(VMachine* vm)
{
    // get the value that holds the address
    Index idx = READ16(vm);
    Value* val = getVal(vm->val_store, idx);

    // get the arithmetic result
    Value* result;
    POP(vm, result);

    if(isFalse(result)) {
        switch(val->type) {
            case VAL_INUM:
                vm->inst->index += val->data.inum;
                break;
            case VAL_ADDRESS:
            case VAL_UNUM:
                vm->inst->index += val->data.unum;
                break;
            case VAL_FNUM:
            case VAL_ERROR:
            case VAL_BOOL:
            case VAL_OBJ:
            default:
                runtimeError("type %s cannot hold an address", valToStr(val->type));
                return 1;
        }
    }

    return 0;
}

int do_OP_JMPIFR8(VMachine* vm)
{
    int addr = (int8_t)READ8(vm);
    Value* val;
    POP(vm, val);

    if(isFalse(val)) {
        vm->inst->index += addr;
    }

    return 0;
}

int do_OP_JMPIFR16(VMachine* vm)
{
    int addr = (int16_t)READ16(vm);
    Value* val;
    POP(vm, val);

    if(isFalse(val)) {
        vm->inst->index += addr;
    }

    return 0;
}

int do_OP_JMPIFR32(VMachine* vm)
{
    int addr = (int32_t)READ32(vm);
    Value* val;
    POP(vm, val);

    if(isFalse(val)) {
        vm->inst->index += addr;
    }

    return 0;
}
