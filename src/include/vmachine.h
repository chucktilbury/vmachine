#ifndef _VM_SUPPORT_H_
#define _VM_SUPPORT_H_

//typedef uint16_t Index;
// forward declaration
//typedef struct _vmachine_ VMachine;

#include "memory.h"
#include "instr.h"
//#include "value.h"
//#include "strings.h"
#include "error.h"
#include "opcodes.h"
#include "file_io.h"
#include "cmd_line.h"
#include "call_stack.h"
#include "val_stack.h"
#include "var_store.h"
#include "var_type.h"
#include "misc.h"


// typedef struct _vmachine_ {
//     // InstStore* inst;
//     // ValList* val_stack;
//     // ValList* val_store;
//     // ValList* cons_store;
//     // StrStore* str_store;
//     // bool zflag;
// } VMachine;

// void runMachine();
// void createVMachine();
// void destroyVMachine();

// #define READ8(vm)   read8InstStore(vm->inst)
// #define READ16(vm)  read16InstStore(vm->inst)
// #define READ32(vm)  read32InstStore(vm->inst)
// #define READ64(vm)  read64InstStore(vm->inst)
// #define WRITE8(vm, word)    write8InstStore(vm->inst, word)
// #define WRITE16(vm, word)   write16InstStore(vm->inst, word)
// #define WRITE32(vm, word)   write32InstStore(vm->inst, word)
// #define WRITE64(vm, word)   write64InstStore(vm->inst, word)
// #define IP(vm)      getIndex(vm->inst)

#endif