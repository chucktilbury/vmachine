#ifndef _VM_SUPPORT_H_
#define _VM_SUPPORT_H_

typedef uint16_t StoreIndex;

#include "value.h"
#include "error.h"
#include "instr.h"
#include "opcodes.h"
#include "instr.h"
#include "strings.h"
#include "utils.h"

typedef struct {
    InstStore* inst;
    ValueStore* val_stack;
    ValueStore* val_store;
    ValueStore* heap;
    StrStore* strs;
} VirtualMachine;

void runMachine(VirtualMachine* vm);
VirtualMachine* createVirtualMachine();
void destroyVirtualMachine(VirtualMachine* vm);

#define READ8(vm)   read8InstStore(vm->inst)
#define READ16(vm)  read16InstStore(vm->inst)
#define READ32(vm)  read32InstStore(vm->inst)
#define READ64(vm)  read64InstStore(vm->inst)
#define WRITE8(vm, word)    write8InstStore(vm->inst, word)
#define WRITE16(vm, word)   write16InstStore(vm->inst, word)
#define WRITE32(vm, word)   write32InstStore(vm->inst, word)
#define WRITE64(vm, word)   write64InstStore(vm->inst, word)
#define IP(vm)      getIndex(vm->inst)

#include "file_io.h"

#endif