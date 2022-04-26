#ifndef OPCODES_H
#define OPCODES_H

typedef enum {
    OP_ERROR,   // payload is a string with the error, set by the VM
    OP_NOOP,    // no operation
    OP_EXIT,    // cause the VM to exit
    OP_SAVE,    // save the top of the stack to the slot number (constant)
    OP_LOCAL,   // save top of stack to a local

    OP_CAST,    // change the type of a variable at a slot number
    OP_PRINTS,  // print the value at the top of the stack
    OP_PRINT,   // print the value given by the index operand

    OP_TRAP,    // (N.I.) immediate is an 8 bit trap number
    OP_EXCEPT,  // (N.I.) raise an exception and process an exception handler (constatant)
    OP_CALLX,   // (N.I.) call an externally defined function (constatant)

    OP_CALL,    // call an absolute address (constatant)
    OP_RETURN,  // return from a call
    OP_JMP,     // jump to an absolute address (constatant)
    OP_JMPIF,   // conditional jump to an absolute address (constatant)

    OP_PUSH,    // push the value slot on the value stack (variable)
    OP_PUSH8,   // push an immediate with type info.
    OP_PUSH16,
    OP_PUSH32,
    OP_POP,     // pop the value from the stack and throw it away
    OP_PEEK,    // look at the value stack without popping

    // comparison operators
    OP_NOT,     // unary not conditional
    OP_EQ,      // equal conditional
    OP_NEQ,     // not equal conditional
    OP_LEQ,     // less-or-equal conditional
    OP_GEQ,     // greater-or-equal conditional
    OP_LESS,    // less than conditional
    OP_GTR,     // greater than conditional

    // arithmetic operators
    OP_NEG,     // unary arithmetic negation
    OP_ADD,     // arithmetic add
    OP_SUB,     // arithmetic subtract
    OP_MUL,     // arithmetic multiply
    OP_DIV,     // arithmetic divide
    OP_MOD,     // arithmetic modulo
} OpCodes;

const char* opToStr(OpCodes op);

#endif

