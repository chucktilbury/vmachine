
* Labels need to be used before they have been defined. This has an effect
on the JMP, JMPIF, and CALL instructions. Do not really want to -require-
using a label. Could use any int or unsigned.

* use the isAssigned flag to pre-declare vars. Check all symbols at the end
to make sure they were all assigned a value.

* Symbol names need to store the line number so that an error message can be
generated if they are never assigned a value.

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