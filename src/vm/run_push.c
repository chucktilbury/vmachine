
#include "common.h"

int do_OP_PUSH(VMachine* vm)
{
    uint16_t idx = READ16(vm);
    COPY(vm, idx); // copy from val_store to val_stack
    return 0;
}

int do_OP_PUSH8(VMachine* vm)
{
    uint8_t type = READ8(vm);
    uint8_t valu = READ8(vm);
    Value* val = createVal(type);
    val->data.unum = (uint32_t)valu;
    pushVal(vm->val_stack, val);
    return 0;
}

int do_OP_PUSH16(VMachine* vm)
{
    uint8_t type = READ8(vm);
    uint16_t valu = READ16(vm);
    Value* val = createVal(type);
    val->data.unum = (uint32_t)valu;
    pushVal(vm->val_stack, val);
    return 0;
}

int do_OP_PUSH32(VMachine* vm)
{
    uint8_t type = READ8(vm);
    uint32_t valu = READ32(vm);
    Value* val = createVal(type);
    val->data.unum = valu;
    pushVal(vm->val_stack, val);
    return 0;
}

int do_OP_POP(VMachine* vm)
{
    Value* obj;
    POP(vm, obj);
    (void)obj;
    return 0;
}

