

* Implement jmpif instructions

* Implement call/return instructions. Do these use the value stack or do we
need a separate stack for them?

* Jump and call instructions that use the top of the stack as an operand. Note
that jmpif already uses the top of the stack and must be implemented in asm
code.

* What about a callif instruction? What about jmp/call if true?

* assembler needs to have the logic to do relative jumps to labels and
automatically select abs or rel jmp.

* mechanism for comparing objects.

* Need a way to delete/free values.

* figure out a way to store symbol tables in the ASM file.

* fix error messages for different executables.

* implement some kind of trace functionality that is settable at runtime.

* print immediate values in exec trace and elsewhere.

* make exec trace switchable on the command line

* Fix the many memory leaks in the assembler.

Build debugger with trace, breakpoints, and watches.