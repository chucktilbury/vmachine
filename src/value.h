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
    VAL_INDEX,
} ValueType;

typedef struct {
    ValueType type;
    const char* name;
    bool isAssigned;
    bool isWritable;
    bool inUse;
    union {
        uint64_t unum;
        int64_t inum;
        double fnum;
        bool boolean;
        char* str;
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
void printValue(Value obj);

void initValue(Value* obj, ValueType type);
void assignValue(Value* obj, ValueType type, void* data);

StoreIndex addValStore(ValueStore* os, Value obj);
Value getValStore(ValueStore* os, StoreIndex index);
Value setValStore(ValueStore* os, StoreIndex index, Value obj);

void pushValStore(ValueStore* os, Value obj);
Value popValStore(ValueStore* os);
Value peekValStore(ValueStore* os);

#define PUSH(vm, obj)   do { pushValStore(vm->val_stack, (obj)); } while(0)
#define COPY(vm, n)     do { pushValStore(vm->val_stack, getValStore(vm->val_store, (n))); } while(0)
#define POP(vm, o)      do { (o) = popValStore(vm->val_stack); } while(0)
#define PEEK(vm, o)     do { (o) = peekValStore(vm->val_stack); } while(0)
#endif