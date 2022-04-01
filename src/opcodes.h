#ifndef _OPCODES_H_
#define _OPCODES_H_

typedef enum {
    OP_NOOP,    // no operation
    OP_EXIT,    // cause the VM to exit
    OP_RETURN,  // return from a call

    // immediate is a 16 bit unsigned indexes into the value store
    // TODO: Implement the call stack
    OP_CALL,    // call an absolute address (constatant)
    OP_CALLX,   // call an externally defined function (constatant)
    OP_JMP,     // jump to an absolute address (constatant)
    OP_JMPIF,   // conditional jump to an absolute address (constatant)
    OP_EXCEPT,  // process an exception handler (constatant)

    OP_SAVE,    // NEW: save the top of the stack to the slot number (constant)
    OP_PUSH,    // push the value slot on the value stack (variable)
    OP_POP,     // pop the value from the stack and throw it away
    OP_ERROR,   // payload is a string with the error, set by the VM

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

    OP_PRINT,   // print the value at the top of the stack
    //OP_TRAP,    // immediate is an 8 bit trap number

    OP_FREE,     // not found a use yet
} OpCodes;

const char* opToStr(OpCodes op);

#endif

/*
 * function parameters and return values.
 *
 * Parameters are pushed on the stack before the call, along with the number
 * of parameters. The call pushes the return address on the stack. Before the
 * return is called, the top of the stack must be the return address. So, we
 * need a register that save the top of the stack so that we can refer to the
 * function parameters using a negative index. Local vars are discarded on
 * the stack. Return value has to be on the stack somewhere. Maybe CALL can
 * reserve a slot on the stack for it. Or there could be a register that
 * holds the return value. In any case, the stack needs to be modeled closer
 * to the real thing.
 *
 * Do I want a separate call stack? Or is the expression stack the proper way
 * to maintain calling information?
 *
 * POP instruction that pops more than one item?
 *
 * What are the differences between constants and variables?
 */