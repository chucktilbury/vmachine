
* Make expressions.c do the whole operation instead of the runner.

* mechanism for comparing objects.

* Need a way to delete/free values.

* Fix casting. (values to pointers)

* Symbol table dereferences pointers instead of an index. Is there some
foolyfoo we can do with pointer values to store in the instruction stream?
Open addressing hash table slots? Implications for saving and loading....

* Need a way to store literals. Literals and variables should not be mixed
in expressions. Situations where they need to mix should be handled in ASM
code. Fix the parser for that.

* figure out a way to store symbol tables in the ASM file.

* Look at converting indexes to pointers and back for load/save.

* When joining together several files, how to fix up the indexes. Almost
have to simulate the whole program....
