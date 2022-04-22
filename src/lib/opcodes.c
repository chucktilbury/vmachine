
#include "opcodes.h"

const char* opToStr(OpCodes op)
{
    return (op == OP_NOOP) ? "NOOP" :
           (op == OP_ERROR) ? "ERROR" :
           (op == OP_EXIT) ? "EXIT" :
           (op == OP_EXCEPT) ? "EXCEPT" :
           (op == OP_LOCAL) ? "LOCAL" :

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
           (op == OP_PRINTS) ? "PRINTS" :

           (op == OP_PUSH) ? "PUSH" :
           (op == OP_PUSH8) ? "PUSH8" :
           (op == OP_PUSH16) ? "PUSH16" :
           (op == OP_PUSH32) ? "PUSH32" :
           (op == OP_POP) ? "POP" :
           (op == OP_PEEK) ? "PEEK" :

           (op == OP_CALLX) ? "CALLX" :
           (op == OP_CALL) ? "CALL" :
           (op == OP_RETURN) ? "RETURN" :
           (op == OP_JMP) ? "JMP" :
           (op == OP_JMPIF) ? "JMPIF" : "UNKNOWN";

    //(op == OP_TRAP)? "OP_TRAP":
    //(op == OP_CONST)? "OP_CONST":
    //(op == OP_FREE)? "OP_FREE":
}

