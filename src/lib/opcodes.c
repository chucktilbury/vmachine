
#include "opcodes.h"

const char* opToStr(OpCodes op)
{
    return (op == OP_NOOP) ? "OP_NOOP" :
           (op == OP_ERROR) ? "OP_ERROR" :
           (op == OP_EXIT) ? "OP_EXIT" :
           (op == OP_CALL) ? "OP_CALL" :
           //(op == OP_CALLR)? "OP_CALLR":
           (op == OP_CALLX) ? "OP_CALLX" :
           (op == OP_RETURN) ? "OP_RETURN" :
           (op == OP_JMP) ? "OP_JMP" :
           //(op == OP_JMPR)? "OP_JMPR":
           (op == OP_JMPIF) ? "OP_JMPIF" :
           //(op == OP_JMPIFR)? "OP_JMPIFR":
           (op == OP_EXCEPT) ? "OP_EXCEPT" :
           (op == OP_PUSH) ? "OP_PUSH" :
           (op == OP_POP) ? "OP_POP" :
           //(op == OP_CONST)? "OP_CONST":
           //(op == OP_FREE)? "OP_FREE":
           (op == OP_EQ) ? "OP_EQ" :
           (op == OP_NEQ) ? "OP_NEQ" :
           (op == OP_LEQ) ? "OP_LEQ" :
           (op == OP_GEQ) ? "OP_GEQ" :
           (op == OP_LESS) ? "OP_LESS" :
           (op == OP_GTR) ? "OP_GTR" :
           (op == OP_ADD) ? "OP_ADD" :
           (op == OP_SUB) ? "OP_SUB" :
           (op == OP_MUL) ? "OP_MUL" :
           (op == OP_DIV) ? "OP_DIV" :
           (op == OP_MOD) ? "OP_MOD" :
           (op == OP_CAST) ? "OP_CAST" :
           (op == OP_SAVE) ? "OP_SAVE" :
           //(op == OP_TRAP)? "OP_TRAP":
           (op == OP_PRINT) ? "OP_PRINT" : "UNKNOWN";
}

