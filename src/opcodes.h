#ifndef _OPCODES_H_
#define _OPCODES_H_

typedef enum {
    OP_ERROR,
    OP_NOOP,
    OP_EXIT,
    OP_CALL,
    OP_CALLX,
    OP_RETURN,
    OP_JMP,
    OP_JMPIF,
    OP_EXCEPT,
    OP_PUSH,
    OP_POP,
    OP_ASSIGN,
    OP_FREE,

    OP_NOT,
    OP_EQ,
    OP_NOT_EQ,
    OP_LESS_EQ,
    OP_GTR_EQ,
    OP_LESS,
    OP_GREATER,

    OP_NEG,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,

    OP_PRINT,
} OpCodes;

const char* opToStr(OpCodes op);

#endif