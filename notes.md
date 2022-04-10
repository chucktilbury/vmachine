

* Need a way to "see" values on the stack. Set up a protocol for calling a
function and  returning a value. Assembler macros?

push v3 = parameter 1
push v2 = parm 2
push v1 = blank slot to hold the return value
call v0 = return address from call. this is top of stack for return

How to refer to parameters in terms of their position on the stack?

Need to pop the 3 parameters after the call. Do I need a single instruction
that can do that? Maybe modify return or pop to accept a parameter.

# POP needs to destroy values when they are popped.

# Prove that values can change at runtime.

* Jump and call instructions that use the top of the stack as an operand. Note
that jmpif already uses the top of the stack and must be implemented in asm
code.

* What about a callif instruction? What about jmp/call if true?

* assembler needs to have the logic to do relative jumps to labels and
automatically select abs or rel jmp.

* mechanism for comparing objects. (implement strings)

* Need a way to delete/free values.

* figure out a way to store symbol tables in the ASM file for debugging.

* fix error messages for different executables.

* implement some kind of trace functionality that is settable at runtime.

* print immediate values in exec trace and elsewhere.

* make exec trace switchable on the command line

* Fix the many memory leaks in the assembler.

* Bitwise operations?

Build debugger with trace, breakpoints, and watches.