
// stand-alone file writer.
#include "common.h"
#include "vm_support.h"

// create a program that does some arithmetic
void create_program(VirtualMachine* vm)
{
    int val;
    Value obj;

    WRITE8(vm, OP_NOOP);
    WRITE8(vm, OP_NOOP);

    initValue(&obj, VAL_INUM);
    obj.data.inum = 123;
    val = addValStore(vm->val_store, obj);
    WRITE8(vm, OP_PUSH);
    WRITE16(vm, val);
    WRITE8(vm, OP_PRINT);

    initValue(&obj, VAL_INUM);
    obj.data.inum = 42;
    val = addValStore(vm->val_store, obj);
    WRITE8(vm, OP_PUSH);
    WRITE16(vm, val);
    WRITE8(vm, OP_PRINT);

    WRITE8(vm, OP_ADD);
    WRITE8(vm, OP_PRINT);

    WRITE8(vm, OP_CONSTANT);
    WRITE16(vm, VAL_INUM);
    WRITE64(vm, 22);
    WRITE8(vm, OP_DIV);
    WRITE8(vm, OP_PRINT);

    WRITE8(vm, OP_POP);
    WRITE8(vm, OP_EXIT);
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "use: %s filename\n", argv[0]);
        exit(1);
    }

    VirtualMachine* vm = createVirtualMachine();
    create_program(vm);
    saveVM(argv[1], vm);
    destroyVirtualMachine(vm);

    return 0;
}