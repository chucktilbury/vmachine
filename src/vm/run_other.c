#include "common.h"
#include "vm.h"


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

int do_OP_PRINTS()
{
    trace(5, "\n");
    trace(5, "TOS: %d\t", getValStackLen());
    printVar(convertValToVar(peekStk()));
    trace(1, "\n");
    return 0;
}

int do_OP_PRINT()
{
    trace(5, "\n");
    printVar(getVar(read16()));
    trace(1, "\n");
    return 0;
}

int do_OP_EXCEPT()
{
    runtimeError("EXCEPT is not implemented");
    return 1;
}

/**
 * @brief The immediate uint16_t is the slot of the var to save the TOS into.
 *
 * @return int
 */
int do_OP_SAVE()
{
    StkVal val = peekStk();
    uint16_t slot = read16();
    assignValToVar(getVar(slot), val);

    return 0;
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

int do_OP_CAST()
{
    uint8_t type = read8();
    uint16_t idx = read16();
    castVar(getVar(idx), type);
    return 0;
}

int do_OP_LOCAL()
{
    short ofst = (short)read16();
    int base = peekCallBase();
    StkVal val = peekStk();
    printVal(val.type, &val.data.inum); fputc('\n', stdout);
    assignVal(ofst+base, val);
    return 0;
}