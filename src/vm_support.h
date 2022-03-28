#ifndef _VM_SUPPORT_H_
#define _VM_SUPPORT_H_

#include "object.h"
#include "error.h"
#include "instr.h"
#include "opcodes.h"
#include "instr.h"

typedef struct {
    InstStore* inst;
    ObjStore* val_stack;
    ObjStore* val_store;
    ObjStore* heap;
} VirtualMachine;

void runMachine(VirtualMachine* vm);
VirtualMachine* createVirtualMachine();
void destroyVirtualMachine(VirtualMachine* vm);

#define read8(vm)   read8InstStore(vm->inst)
#define read16(vm)  read16InstStore(vm->inst)
#define read32(vm)  read32InstStore(vm->inst)
#define write8(vm, word)    write8InstStore(vm->inst, word)
#define write16(vm, word)   write16InstStore(vm->inst, word)
#define write32(vm, word)   write32InstStore(vm->inst, word)


#include "file_io.h"

#endif