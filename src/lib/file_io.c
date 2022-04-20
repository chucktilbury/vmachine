
#include "common.h"

void loadVM(const char* fname)
{
    FILE* infile_pointer = fopen(fname, "r");
    assert(infile_pointer != NULL);

    loadInstStore(infile_pointer);
    loadVarStore(infile_pointer);
    createValStack();
    createCallStack();

    fclose(infile_pointer);
}

void saveVM(const char* fname)
{
    FILE* outfile_pointer = fopen(fname, "w");
    assert(outfile_pointer != NULL);

    saveInstStore(outfile_pointer);
    saveVarStore(outfile_pointer);

    fclose(outfile_pointer);
}


