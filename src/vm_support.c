
#include "common.h"
#include "vm_support.h"

VirtualMachine* createVirtualMachine()
{
    VirtualMachine* vm = malloc(sizeof(VirtualMachine));
    assert(vm != NULL);

    vm->inst = createInstStore();
    vm->val_stack = createObjStore();
    vm->val_store = createObjStore();
    vm->heap = createObjStore();

    return vm;
}

void destroyVirtualMachine(VirtualMachine* vm)
{
    if(vm != NULL) {
        destroyInstStore(vm->inst);
        destroyObjStore(vm->val_stack);
        destroyObjStore(vm->val_store);
        destroyObjStore(vm->heap);
        free(vm);
    }
}
