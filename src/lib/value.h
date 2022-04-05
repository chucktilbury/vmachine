#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdint.h>
#include <stddef.h>
#include "vmachine.h"

typedef enum {
    VAL_ERROR,
    VAL_UNUM,
    VAL_INUM,
    VAL_FNUM,
    VAL_BOOL,
    VAL_OBJ,
    VAL_ADDRESS,
} ValType;

typedef struct {
    ValType type;
    bool isAssigned;// has a value
    bool isConst;   // declared as constant
    bool isLiteral; // not connected to a symbol
    union {
        uint64_t unum;
        int64_t inum;
        double fnum;
        bool boolean;
        Index obj;
    } data;
} Value;

typedef GenericPtrList ValList;

#define destroyValList(vl)  destroyGPL(vl)
#define addVal(vl, obj)     addGPL(vl, obj)
#define getVal(vl, idx)     getGPL(vl, idx)
#define setVal(vl, idx, obj)    setGPL(vl, idx, obj)
#define pushVal(vl, obj)    pushGPL(vl, obj)
#define popVal(vl)          popGPL(vl)
#define peekVal(vl)         peekGPL(vl)

ValList* createValList();
Value* createVal(ValType type);
void assignVal(Value* obj, Value* val);
void dumpVals();
void printVal(Value* obj);
const char* valToStr(ValType type);

#define PUSH(vm, obj)   do { pushVal(vm->val_stack, (obj)); } while(0)
#define COPY(vm, n)     do { pushVal(vm->val_stack, getVal(vm->val_store, (n))); } while(0)
#define POP(vm, o)      do { (o) = popVal(vm->val_stack); } while(0)
#define PEEK(vm, o)     do { (o) = peekVal(vm->val_stack); } while(0)
#define VAL(store, idx) ((store).list[idx])

#endif