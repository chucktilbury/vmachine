
#include "opcodes.h"

const char* opToStr(OpCodes op)
{
    return (op == OP_NOOP) ? "OP_NOOP" :
           (op == OP_ERROR) ? "OP_ERROR" :
           (op == OP_EXIT) ? "OP_EXIT" :
           (op == OP_EXCEPT) ? "OP_EXCEPT" :

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
           (op == OP_PRINT) ? "OP_PRINT" :

           (op == OP_PUSH) ? "OP_PUSH" :
           (op == OP_PUSH8) ? "OP_PUSH8" :
           (op == OP_PUSH16) ? "OP_PUSH16" :
           (op == OP_PUSH32) ? "OP_PUSH32" :
           (op == OP_POP) ? "OP_POP" :

           (op == OP_CALLX) ? "OP_CALLX" :
           (op == OP_CALL) ? "OP_CALL" :
           (op == OP_CALL8) ? "OP_CALL8" :
           (op == OP_CALL16) ? "OP_CALL16" :
           (op == OP_CALL32) ? "OP_CALL32" :
           (op == OP_RETURN) ? "OP_RETURN" :
           (op == OP_CALLR) ? "OP_CALLR" :
           (op == OP_CALLR8) ? "OP_CALLR8" :
           (op == OP_CALLR16) ? "OP_CALLR16" :
           (op == OP_CALLR32) ? "OP_CALLR32" :

           (op == OP_JMP) ? "OP_JMP" :
           (op == OP_JMP8) ? "OP_JMP8" :
           (op == OP_JMP16) ? "OP_JMP16" :
           (op == OP_JMP32) ? "OP_JMP32" :
           (op == OP_JMPR) ? "OP_JMPR" :
           (op == OP_JMPR8) ? "OP_JMPR8" :
           (op == OP_JMPR16) ? "OP_JMPR16" :
           (op == OP_JMPR32) ? "OP_JMPR32" :

           (op == OP_JMPIF) ? "OP_JMPIF" :
           (op == OP_JMPIF8) ? "OP_JMPIF8" :
           (op == OP_JMPIF16) ? "OP_JMPIF16" :
           (op == OP_JMPIF32) ? "OP_JMPIF32" :
           (op == OP_JMPIFR) ? "OP_JMPIFR" :
           (op == OP_JMPIFR8) ? "OP_JMPIFR8" :
           (op == OP_JMPIFR16) ? "OP_JMPIFR16" :
           (op == OP_JMPIFR32) ? "OP_JMPIFR32" :  "UNKNOWN";

           //(op == OP_TRAP)? "OP_TRAP":
           //(op == OP_CONST)? "OP_CONST":
           //(op == OP_FREE)? "OP_FREE":
}

