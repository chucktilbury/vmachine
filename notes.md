

# Current

* Print immediate values in exec trace and elsewhere. Controlled by a verbosity command line parameters.

* The assembler should always output a preamble that jumps to a location in the code and exits the program. The entry location should be given by a "standard" symbol, such as "main" or "entry". If the symbol has not been defined, then an error should be published and no code is generated.

* Add a command parameter to the assembler that forces code to be output, even if there are errors.
    * Stubbed it out.

* Add command line to vm that causes the trace to be written to disk instead of stdout.
    * Figure out how the print statements work with that. (stubbed out)

# Random

* Runtime library that includes things like fopen() and friends.

* Mechanism for comparing objects. (implement strings)

* Figure out a way to store symbol tables in the ASM file for debugging.

* Bitwise operations?

* Callif instructions?

* Make command line parser capable of using concatenated switches and not having to put a space between a parameter and its args.

* Fix memory allocation routines to detect and report memory leaks. This feature should be disabled for "release" builds.

# Future

* Reduce the library code to a few headers if possible.

* Need to have the assembler set the entry point to the program with a dedicated symbol.

* Integrate the disassembler into the assembler and have it generate a "listing" from the command line parameters. Integrate the symbol table into this.

* The assembler can accept a list of files. Have it maintain the parser state when switching so that all of the files are part of the same output. Push the files on the file stack in the reverse order from the command line.

* Build debugger with trace, breakpoints, and watches.

* Automate testing.

# Completed

* Prove that values can change at runtime.

* Fix expression stack so that it does not allocate data. The value structs are assigned as if they were an int instead of as a pointer. Stack values are not the same as the value store. They are more simple.

* Create a separate call stack that maintains the value stack position for the parameters and local vars. The return address is also returned on this stack in such a ways as to only require a simple assignment instead of passing pointers around.

* Get rid of the VM data structure and have its elements run by the modules that manage it. This will help get rid of some indirection.

* Move the disassembler into the assembler and create a listing according to the command line parameters.

* Integrate labels into the listing in the assembler.

* Clean up comp_expr.c error messages.

* Use the linker to link the correct error function in common code.

* Implement some kind of trace functionality that is settable at runtime.

* Assembler needs to have the logic to do relative jumps to labels and automatically select abs or rel jmp.
    * Update: Is this needed? Or is it acceptable to always have a label (in the symbol table) for a jump or call destination?
    * Deleted relative and immediate jumps.

