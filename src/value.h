#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdint.h>
#include <stddef.h>

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
        int index;
        char* str;
    } data;
} Value;

typedef struct {
    Value* list;
    int cap;
    int len;
} ValueStore;

ValueStore* createObjStore();
void destroyObjStore(ValueStore* os);
const char* valToStr(ValueType type);
void printValue(Value obj);

void initObj(Value* obj, ValueType type);
void assignObj(Value* obj, ValueType type, void* data);

int addObjStore(ValueStore* os, Value obj);
Value getObjStore(ValueStore* os, int index);
Value setObjStore(ValueStore* os, int index, Value obj);

void pushObjStore(ValueStore* os, Value obj);
Value popObjStore(ValueStore* os);
Value peekObjStore(ValueStore* os);

#define PUSH(vm, obj)   do { pushObjStore(vm->val_stack, (obj)); } while(0)
#define COPY(vm, n)     do { pushObjStore(vm->val_stack, getObjStore(vm->val_store, (n))); } while(0)
#define POP(vm, o)      do { (o) = popObjStore(vm->val_stack); } while(0)
#define PEEK(vm, o)     do { (o) = peekObjStore(vm->val_stack); } while(0)
#endif