
#include "common.h"

#include "run_arith.h"
#include "run_call.h"
#include "run_comp.h"
#include "run_jmp.h"
#include "run_jmpif.h"
#include "run_other.h"
#include "run_push.h"

typedef int (*do_opcode_func)();

do_opcode_func opcode_table[] = {
    [OP_NOOP] = do_OP_NOOP,
    [OP_EXIT] = do_OP_EXIT,
    [OP_RETURN] = do_OP_RETURN,
    [OP_CALL] = do_OP_CALL,
    [OP_CALL8] = do_OP_CALL8,
    [OP_CALL16] = do_OP_CALL16,
    [OP_CALL32] = do_OP_CALL32,
    [OP_JMP] = do_OP_JMP,
    [OP_JMP8] = do_OP_JMP8,
    [OP_JMP16] = do_OP_JMP16,
    [OP_JMP32] = do_OP_JMP32,
    [OP_JMPIF] = do_OP_JMPIF,
    [OP_JMPIF8] = do_OP_JMPIF8,
    [OP_JMPIF16] = do_OP_JMPIF16,
    [OP_JMPIF32] = do_OP_JMPIF32,
    [OP_CALLR] = do_OP_CALLR,
    [OP_CALLR8] = do_OP_CALLR8,
    [OP_CALLR16] = do_OP_CALLR16,
    [OP_CALLR32] = do_OP_CALLR32,
    [OP_JMPR] = do_OP_JMPR,
    [OP_JMPR8] = do_OP_JMPR8,
    [OP_JMPR16] = do_OP_JMPR16,
    [OP_JMPR32] = do_OP_JMPR32,
    [OP_JMPIFR] = do_OP_JMPIFR,
    [OP_JMPIFR8] = do_OP_JMPIFR8,
    [OP_JMPIFR16] = do_OP_JMPIFR16,
    [OP_JMPIFR32] = do_OP_JMPIFR32,
    [OP_PUSH] = do_OP_PUSH,
    [OP_PUSH8] = do_OP_PUSH8,
    [OP_PUSH16] = do_OP_PUSH16,
    [OP_PUSH32] = do_OP_PUSH32,
    [OP_EXCEPT] = do_OP_EXCEPT,
    [OP_CALLX] = do_OP_CALLX,
    [OP_SAVE] = do_OP_SAVE,
    [OP_POP] = do_OP_POP,
    [OP_ERROR] = do_OP_ERROR,
    [OP_NOT] = do_OP_NOT,
    [OP_EQ] = do_OP_EQ,
    [OP_NEQ] = do_OP_NEQ,
    [OP_LEQ] = do_OP_LEQ,
    [OP_GEQ] = do_OP_GEQ,
    [OP_LESS] = do_OP_LESS,
    [OP_GTR] = do_OP_GTR,
    [OP_NEG] = do_OP_NEG,
    [OP_ADD] = do_OP_ADD,
    [OP_SUB] = do_OP_SUB,
    [OP_MUL] = do_OP_MUL,
    [OP_DIV] = do_OP_DIV,
    [OP_MOD] = do_OP_MOD,
    [OP_CAST] = do_OP_CAST,
    [OP_PRINT] = do_OP_PRINT,
};
#define NUM_OPCODES (sizeof(opcode_table) / sizeof(do_opcode_func))

void runMachine(VMachine* vm)
{
    bool finished = false;
    uint32_t opcode = 0;

    while(!finished) {
        VTRACE(5, "%04d ", IP(vm));
        opcode = READ8(vm);
        if(opcode > NUM_OPCODES) {
            runtimeError("invalid opcode: 0x%02X at 0x%08X", opcode, IP(vm));
            finished = true;
        }
        else {
            VTRACE(5, "%-10s\t", opToStr(opcode));
            finished = (*opcode_table[opcode])(vm);
        }
        VTRACE(5, "\n");
    }
}

