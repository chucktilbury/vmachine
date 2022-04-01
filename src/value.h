#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdint.h>
#include <stddef.h>
#include "vm_support.h"

typedef enum {
    VAL_ERROR,
    VAL_UNUM,
    VAL_INUM,
    VAL_FNUM,
    VAL_BOOL,
    VAL_STRING,
    VAL_ADDRESS,
} ValueType;

typedef struct {
    ValueType type;
    bool isAssigned;
    bool isConst;
    union {
        uint64_t unum;
        int64_t inum;
        double fnum;
        bool boolean;
        Index obj;
    } data;
} Value;

typedef struct {
    Value* list;
    int cap;
    int len;
} ValueStore;

ValueStore* createValStore();
void destroyValStore(ValueStore* os);
const char* valToStr(ValueType type);

void initVal(Value* obj, ValueType type);
void assignVal(Value* obj, Value* val);

Index addVal(ValueStore* os, Value obj);
Value getVal(ValueStore* os, Index idx);
Value setVal(ValueStore* os, Index idx, Value obj);

void pushVal(ValueStore* os, Value obj);
Value popVal(ValueStore* os);
Value peekVal(ValueStore* os);

#define PUSH(vm, obj)   do { pushVal(vm->val_stack, (obj)); } while(0)
#define COPY(vm, n)     do { pushVal(vm->val_stack, getVal(vm->val_store, (n))); } while(0)
#define POP(vm, o)      do { (o) = popVal(vm->val_stack); } while(0)
#define PEEK(vm, o)     do { (o) = peekVal(vm->val_stack); } while(0)
#define VAL(store, idx) ((store).list[idx])

#endif