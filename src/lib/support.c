
#include "common.h"
#include "vmachine.h"



VMachine* createVMachine()
{
    VMachine* vm = _alloc_ds(VMachine);

    vm->inst = createInstStore();
    vm->val_stack = createValList();
    vm->val_store = createValList();
    vm->cons_store = createValList();
    vm->str_store = createStrStore();

    return vm;
}

void destroyVMachine(VMachine* vm)
{
    if(vm != NULL) {
        destroyInstStore(vm->inst);
        destroyValList(vm->val_stack);
        destroyValList(vm->val_store);
        destroyValList(vm->cons_store);
        destroyStrStore(vm->str_store);
        _free(vm);
    }
}

