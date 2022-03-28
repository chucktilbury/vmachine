
// stand-alone file writer.
#include "common.h"
#include "vm_support.h"

// create a program that does some arithmetic
void create_program(VirtualMachine* vm)
{
    int val;
    Object obj;

    write8(vm, OP_NOOP);
    write8(vm, OP_NOOP);

    initObj(&obj, OBJ_INUM);
    obj.data.inum = 123;
    val = addObjStore(vm->val_store, obj);
    write8(vm, OP_PUSH);
    write16(vm, val);

    initObj(&obj, OBJ_INUM);
    obj.data.inum = 42;
    val = addObjStore(vm->val_store, obj);
    write8(vm, OP_PUSH);
    write16(vm, val);

    write8(vm, OP_ADD);
    write8(vm, OP_PRINT);
    write8(vm, OP_EXIT);
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