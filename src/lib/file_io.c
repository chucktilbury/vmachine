
#include "common.h"

typedef struct {
    uint16_t type;
    bool isAssigned;
    bool isConst;
    bool isLiteral;
    uint32_t hash;
    uint32_t value;
} val_fmt;

typedef struct {
    size_t sstore_len;
    size_t vstore_len;
    size_t instr_len;
} vm_fmt;

static void save_values(ValList* store, FILE* fp)
{
    for(int idx = 0; idx < store->len; idx++) {
        val_fmt vfmt;
        Value* val = store->list[idx];
        vfmt.type = val->type;
        vfmt.isAssigned = val->isAssigned;
        vfmt.isConst = val->isConst;
        vfmt.isLiteral = val->isLiteral;
        vfmt.hash = val->hash;
        vfmt.value = val->data.unum;
        fwrite(&vfmt, sizeof(val_fmt), 1, fp);
    }
}

static void load_values(size_t len, ValList* store, FILE* fp)
{
    // load the value store
    store->len = len;
    while(store->len >= store->cap) {
        store->cap <<= 1;
    }
    store->list = _realloc_ds_array(store->list, void*, store->cap);
    for(int idx = 0; idx < store->len; idx++) {
        val_fmt valf;
        fread(&valf, sizeof(val_fmt), 1, fp);
        Value* val = createVal(valf.type);
        val->isAssigned = valf.isAssigned;
        val->isConst = valf.isConst;
        val->isLiteral = valf.isLiteral;
        val->hash = valf.hash;
        val->data.unum = valf.value;
        store->list[idx] = val;
    }
}

void saveVM(VMachine* vm, const char* fname)
{
    FILE* outfile_pointer = fopen(fname, "wb");
    assert(outfile_pointer != NULL);

    vm_fmt header;
    header.sstore_len = vm->str_store->len;
    header.vstore_len = vm->val_store->len;
    header.instr_len = vm->inst->len;
    fwrite(&header, sizeof(vm_fmt), 1, outfile_pointer);

    // save the string store
    for(size_t idx = 0; idx < vm->str_store->len; idx++) {
        uint16_t len = vm->str_store->list[idx].len;
        fwrite(&len, sizeof(uint16_t), 1, outfile_pointer);
        if(vm->str_store->list[idx].len != 0) {
            fwrite(vm->str_store->list[idx].str, sizeof(char), len, outfile_pointer);
        }
    }

    // save the value store
    save_values(vm->val_store, outfile_pointer);

    // save the instruction store
    fwrite(vm->inst->list, sizeof(uint8_t), vm->inst->len, outfile_pointer);

    fclose(outfile_pointer);
}

VMachine* loadVM(const char* fname)
{

    VMachine* vm = createVMachine();

    FILE* infile_pointer = fopen(fname, "rb");
    assert(infile_pointer != NULL);

    vm_fmt vmf;
    fread(&vmf, sizeof(vm_fmt), 1, infile_pointer);

    // load the string store
    vm->val_store->len = vmf.vstore_len;
    while(vm->str_store->len >= vm->str_store->cap) {
        vm->str_store->cap <<= 1;
    }
    vm->str_store->list = _realloc_ds_array(vm->str_store->list, ObjString, vm->str_store->cap);
    for(size_t idx = 0; idx < vm->str_store->len; idx++) {
        uint16_t len;
        fread(&len, sizeof(uint16_t), 1, infile_pointer);
        vm->str_store->list[idx].len = len;
        if(len != 0) {
            vm->str_store->list[idx].str = _malloc(len);
            fread((void*)vm->str_store->list[idx].str, sizeof(uint8_t), len, infile_pointer);
        }
        else {
            vm->str_store->list[idx].str = NULL;
        }
    }

    // load the value store
    load_values(vmf.vstore_len, vm->val_store, infile_pointer);

    // load the instruction store
    vm->inst->len = vmf.instr_len;
    while(vm->inst->len >= vm->inst->cap) {
        vm->inst->cap <<= 1;
    }
    vm->inst->list = _realloc_ds_array(vm->inst->list, uint8_t, vm->inst->cap);
    fread(vm->inst->list, sizeof(uint8_t), vm->inst->len, infile_pointer);

    fclose(infile_pointer);
    return vm;
}

/*
void writeObjStore(FILE* fp, ValList* os)
{
    fwrite(&os->len, sizeof(uint32_t), 1, fp);
    fwrite(os->list, sizeof(Value), os->len, fp);
}

void readObjStore(FILE* fp, ValList* os)
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

void writeStrStore(FILE* fp, StrStore* ss)
{

}

void readStrStore(FILE* fp, StrStore* ss)
{

}


VMachine* loadVM(const char* fname)
{
    VMachine* vm = createVirtualMachine();

    FILE* infile_pointer = fopen(fname, "rb");
    assert(infile_pointer != NULL);

    readObjStore(infile_pointer, vm->val_store);
    readInstStore(infile_pointer, vm->inst);

    fclose(infile_pointer);
    return vm;
}

void saveVM(const char* fname, VMachine* vm)
{
    FILE* outfile_pointer = fopen(fname, "wb");
    assert(outfile_pointer != NULL);

    writeObjStore(outfile_pointer, vm->val_store);
    writeInstStore(outfile_pointer, vm->inst);

    fclose(outfile_pointer);
}
*/