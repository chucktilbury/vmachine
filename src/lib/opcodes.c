
#include "opcodes.h"

const char* opToStr(OpCodes op)
{
    return (op == OP_NOOP) ? "NOOP" :
           (op == OP_ERROR) ? "ERROR" :
           (op == OP_EXIT) ? "EXIT" :
           (op == OP_EXCEPT) ? "EXCEPT" :

           (op == OP_EQ) ? "EQ" :
           (op == OP_NEQ) ? "NEQ" :
           (op == OP_LEQ) ? "LEQ" :
           (op == OP_GEQ) ? "GEQ" :
           (op == OP_LESS) ? "LESS" :
           (op == OP_GTR) ? "GTR" :

           (op == OP_ADD) ? "ADD" :
           (op == OP_SUB) ? "SUB" :
           (op == OP_MUL) ? "MUL" :
           (op == OP_DIV) ? "DIV" :
           (op == OP_MOD) ? "MOD" :
           (op == OP_CAST) ? "CAST" :

           (op == OP_SAVE) ? "SAVE" :
           (op == OP_PRINT) ? "PRINT" :

           (op == OP_PUSH) ? "PUSH" :
           (op == OP_PUSH8) ? "PUSH8" :
           (op == OP_PUSH16) ? "PUSH16" :
           (op == OP_PUSH32) ? "PUSH32" :
           (op == OP_POP) ? "POP" :

           (op == OP_CALLX) ? "CALLX" :
           (op == OP_CALL) ? "CALL" :
           (op == OP_CALL8) ? "CALL8" :
           (op == OP_CALL16) ? "CALL16" :
           (op == OP_CALL32) ? "CALL32" :
           (op == OP_RETURN) ? "RETURN" :
           (op == OP_CALLR) ? "CALLR" :
           (op == OP_CALLR8) ? "CALLR8" :
           (op == OP_CALLR16) ? "CALLR16" :
           (op == OP_CALLR32) ? "CALLR32" :

           (op == OP_JMP) ? "JMP" :
           (op == OP_JMP8) ? "JMP8" :
           (op == OP_JMP16) ? "JMP16" :
           (op == OP_JMP32) ? "JMP32" :
           (op == OP_JMPR) ? "JMPR" :
           (op == OP_JMPR8) ? "JMPR8" :
           (op == OP_JMPR16) ? "JMPR16" :
           (op == OP_JMPR32) ? "JMPR32" :

           (op == OP_JMPIF) ? "JMPIF" :
           (op == OP_JMPIF8) ? "JMPIF8" :
           (op == OP_JMPIF16) ? "JMPIF16" :
           (op == OP_JMPIF32) ? "JMPIF32" :
           (op == OP_JMPIFR) ? "JMPIFR" :
           (op == OP_JMPIFR8) ? "JMPIFR8" :
           (op == OP_JMPIFR16) ? "JMPIFR16" :
           (op == OP_JMPIFR32) ? "JMPIFR32" :  "UNKNOWN";

    //(op == OP_TRAP)? "OP_TRAP":
    //(op == OP_CONST)? "OP_CONST":
    //(op == OP_FREE)? "OP_FREE":
}

