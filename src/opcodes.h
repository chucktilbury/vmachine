#ifndef _OPCODES_H_
#define _OPCODES_H_

typedef enum {
    OP_NOOP,    // no operation
    OP_EXIT,    // cause the VM to exit
    OP_RETURN,  // return from a call

    // immediate is a 16 bit unsigned indexes into the value store
    OP_CALL,    // call an absolute address
    OP_CALLR,   // call a signed relative offset
    OP_CALLX,   // call an externally defined function
    OP_JMP,     // jump to an absolute address
    OP_JMPR,    // jump to a signed relative address
    OP_JMPIF,   // conditional jump to an absolute address
    OP_JMPIFR,  // conditional jump to a signed relative address
    OP_EXCEPT,  // process an exception handler

    OP_PUSH,    // push the value on the value stack
    OP_POP,     // pop the value from the stack and throw it away
    OP_ERROR,   // payload is a string with the error

    // comparison operators
    OP_NOT,     // not conditional
    OP_EQ,      // equal conditional
    OP_NEQ,     // not equal conditional
    OP_LEQ,     // less-or-equal conditional
    OP_GEQ,     // greater-or-equal conditional
    OP_LESS,    // less than conditional
    OP_GTR,     // greater than conditional

    // arithmetic operators
    OP_NEG,     // arithmetic negation
    OP_ADD,     // arithmetic add
    OP_SUB,     // arithmetic subtract
    OP_MUL,     // arithmetic multiply
    OP_DIV,     // arithmetic divide
    OP_MOD,     // arithmetic modulo

    OP_PRINT,   // print the value at the top of the stack
    //OP_TRAP,    // immediate is an 8 bit trap number

    OP_CONSTANT, // replaced by push
    OP_FREE,     // not found a use yet
} OpCodes;

const char* opToStr(OpCodes op);

#endif