#ifndef _OPCODES_H_
#define _OPCODES_H_

typedef enum {
    OP_ERROR,   // payload is a string with the error, set by the VM
    OP_NOOP,    // no operation
    OP_EXIT,    // cause the VM to exit
    OP_SAVE,    // save the top of the stack to the slot number (constant)

    OP_CAST,    // change the type of a variable at a slot number
    OP_PRINTS,  // print the value at the top of the stack
    OP_PRINT,   // print the value given by the index operand

    OP_TRAP,    // (N.I.) immediate is an 8 bit trap number
    OP_EXCEPT,  // (N.I.) raise an exception and process an exception handler (constatant)
    OP_CALLX,   // (N.I.) call an externally defined function (constatant)

    OP_CALL,    // call an absolute address (constatant)
    OP_CALL8,
    OP_CALL16,
    OP_CALL32,
    OP_CALLR,    // call a signed relative address (constatant)
    OP_CALLR8,
    OP_CALLR16,
    OP_CALLR32,
    OP_RETURN,  // return from a call

    OP_JMP,     // jump to an absolute address (constatant)
    OP_JMP8,
    OP_JMP16,
    OP_JMP32,
    OP_JMPR,     // jump to a signed relative address (constatant)
    OP_JMPR8,
    OP_JMPR16,
    OP_JMPR32,

    OP_JMPIF,   // conditional jump to an absolute address (constatant)
    OP_JMPIF8,
    OP_JMPIF16,
    OP_JMPIF32,
    OP_JMPIFR,   // conditional jump to a signed relative address (constatant)
    OP_JMPIFR8,
    OP_JMPIFR16,
    OP_JMPIFR32,

    OP_PUSH,    // push the value slot on the value stack (variable)
    OP_PUSH8,
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