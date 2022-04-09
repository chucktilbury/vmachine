

* Implement jmpif instructions

* Implement call instructions

* assembler needs to have the logic to do relative jumps to labels and
automatically select abs or rel jmp.

* mechanism for comparing objects.

* Need a way to delete/free values.

* figure out a way to store symbol tables in the ASM file.

* fix error messages for different executables.

* implement some kind of trace functionality that is settable at runtime.

* print immediate values in exec trace and elsewhere.

* make exec trace switchable on the command line

Build debugger with trace, breakpoints, and watches.