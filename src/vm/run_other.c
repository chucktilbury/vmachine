#include "common.h"

/**
 * @brief Do nothing
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_NOOP()
{
    return 0; // do nothing
}

/**
 * @brief Quit the virtual machine loop
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_EXIT()
{
    return 1;   // that's all!
}

int do_OP_PRINT()
{
    printf("\n");
    printVar(convertValToVar(peekVal()));
    printf("\n");
    return 0;
}

int do_OP_EXCEPT()
{
    runtimeError("EXCEPT is not implemented");
    return 1;
}

int do_OP_SAVE()
{
    runtimeError("SAVE is not implemented");
    return 1;
}

int do_OP_ERROR()
{
    runtimeError("encountered error instruction");
    return 1;
}

int do_OP_TRAP()
{
    runtimeError("TRAP is not implemented");
    return 1;
}