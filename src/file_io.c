
#include "common.h"
#include "file_io.h"

void writeObjStore(FILE* fp, ValueStore* os)
{
    fwrite(&os->len, sizeof(uint32_t), 1, fp);
    fwrite(os->list, sizeof(Value), os->len, fp);
}

void readObjStore(FILE* fp, ValueStore* os)
{
    fread(&os->len, sizeof(uint32_t), 1, fp);

    while(os->cap < os->len)
        os->cap <<= 1;

    os->list = realloc(os->list, sizeof(Value)*os->cap);
    assert(os->list != NULL);
    fread(os->list, sizeof(Value), os->len, fp);
}

void writeInstStore(FILE* fp, InstStore* is)
{
    fwrite(&is->len, sizeof(uint32_t), 1, fp);
    fwrite(is->list, sizeof(uint8_t), is->len, fp);
}

void readInstStore(FILE* fp, InstStore* is)
{
    fread(&is->len, sizeof(uint32_t), 1, fp);

    while(is->cap < is->len)
        is->cap <<= 1;

    is->list = realloc(is->list, sizeof(Value)*is->cap);
    assert(is->list != NULL);
    fread(is->list, sizeof(InstStore), is->len, fp);
}

VirtualMachine* loadVM(const char* fname)
{
    VirtualMachine* vm = createVirtualMachine();

    FILE* infile_pointer = fopen(fname, "rb");
    assert(infile_pointer != NULL);

    readObjStore(infile_pointer, vm->val_store);
    readInstStore(infile_pointer, vm->inst);

    fclose(infile_pointer);
    return vm;
}

void saveVM(const char* fname, VirtualMachine* vm)
{
    FILE* outfile_pointer = fopen(fname, "wb");
    assert(outfile_pointer != NULL);

    writeObjStore(outfile_pointer, vm->val_store);
    writeInstStore(outfile_pointer, vm->inst);

    fclose(outfile_pointer);
}
