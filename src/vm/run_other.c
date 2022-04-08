#include "common.h"

/**
 * @brief Do nothing
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_NOOP(VMachine* vm)
{
    (void)vm;
    return 0; // do nothing
}

/**
 * @brief Quit the virtual machine loop
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_EXIT(VMachine* vm)
{
    (void)vm;
    return 1;   // that's all!
}

int do_OP_PRINT(VMachine* vm)
{
    Value* result;
    PEEK(vm, result);
    printVal(result);
    return 0;
}

int do_OP_EXCEPT(VMachine* vm)
{
    Index idx = READ16(vm);
    return 0;
}

int do_OP_SAVE(VMachine* vm)
{
    Index idx = READ16(vm);
    Value* val;
    PEEK(vm, val);
    assignVal(vm->val_store->list[idx], val);
    return 0;
}

int do_OP_ERROR(VMachine* vm)
{
    (void)vm;
    runtimeError("encountered error instruction");
    return 1;
}