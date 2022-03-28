
#include "opcodes.h"

const char* opToStr(OpCodes op)
{
    return (op == OP_NOOP)? "OP_NOOP":
            (op == OP_ERROR)? "OP_ERROR":
            (op == OP_EXIT)? "OP_EXIT":
            (op == OP_CALL)? "OP_CALL":
            (op == OP_CALLX)? "OP_CALLX":
            (op == OP_RETURN)? "OP_RETURN":
            (op == OP_JMP)? "OP_JMP":
            (op == OP_JMPIF)? "OP_JMPIF":
            (op == OP_EXCEPT)? "OP_EXCEPT":
            (op == OP_PUSH)? "OP_PUSH":
            (op == OP_POP)? "OP_POP":
            (op == OP_ALLOC)? "OP_ALLOC":
            (op == OP_FREE)? "OP_FREE":
            (op == OP_EQ)? "OP_EQ":
            (op == OP_NOT_EQ)? "OP_NOT_EQ":
            (op == OP_LESS_EQ)? "OP_LESS_EQ":
            (op == OP_GTR_EQ)? "OP_GTR_EQ":
            (op == OP_LESS)? "OP_LESS":
            (op == OP_GREATER)? "OP_GREATER":
            (op == OP_ADD)? "OP_ADD":
            (op == OP_SUB)? "OP_SUB":
            (op == OP_MUL)? "OP_MUL":
            (op == OP_DIV)? "OP_DIV":
            (op == OP_MOD)? "OP_MOD":
            (op == OP_PRINT)? "OP_PRINT": "UNKNOWN";
}

