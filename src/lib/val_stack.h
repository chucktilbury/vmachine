/**
 * @file val_stack.h
 * @brief Value stack implementation.
 *
 * @version 0.1
 * @date 2022-04-12
 *
 * The value stack is used for evaluating expressions. The values that go in
 * the stack are never allocated into the heap, but are copied around like an
 * int. The value struct is small enough to be cast to a uint64_t without any
 * loss of information. The value stack also implements side stacks that allow
 * things like return addresses and parameter pointers to be stored
 * transparently. This is why it is a separate implementation from Values.
 */
#ifndef _VAL_STACK_H_
#define _VAL_STACK_H_


#include <stddef.h>
#include <stdint.h>

typedef struct _value_ {
    uint8_t type;
    union {
        uint32_t unum; // these 3 have different names for clarity
        uint32_t addr;
        int32_t inum;
        float fnum;
        uint8_t boolean;
        void* obj;
    } data;
} __attribute__((__packed__)) StkVal;

typedef struct _val_stack_ {
    StkVal* list;
    size_t cap;
    size_t len;
} ValStack;

void createValStack();
void destroyValStack();
int getValStackLen();
StkVal initVal(uint16_t type, void* val);
void pushVal(StkVal val);
StkVal popVal();
StkVal peekVal();
//void printVal(StkVal val);
void printVal(uint8_t type, void* val);

// Variable* valToVar(StkVal val);
// StkVal varToVal(Variable* var);

#endif