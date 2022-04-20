
#include "common.h"
#include "vm.h"


/**
 * @brief Jump to the absolute address given by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP()
{
    uint16_t idx = read16();
    Variable* var = getVar(idx);
    setIndex(var->data.addr);
    return 0;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP8()
{
    runtimeError("JMP8 not implemented");
    return 1;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP16()
{
    runtimeError("JMP16 not implemented");
    return 1;
}

/**
 * @brief Jump to the absolute address given by the immediate unsigned int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMP32()
{
    runtimeError("JMP32 not implemented");
    return 1;
}

/**
 * @brief Jump to the relative address given by the immediate variable index.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR()
{
    runtimeError("JMPR not implemented");
    return 1;
}

/**
 * @brief Jump the relative address given by the immediate signed char.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR8()
{
    runtimeError("JMPR8 not implemented");
    return 1;
}

/**
 * @brief Jump the relative address given by the immediate signed short.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR16()
{
    runtimeError("JMPR16 not implemented");
    return 1;
}

/**
 * @brief Jump the relative address given by the immediate signed int.
 *
 * @param opcode
 *
 * @return int
 */
int do_OP_JMPR32()
{
    runtimeError("JMPR32 not implemented");
    return 1;
}
