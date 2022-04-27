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
#ifndef VAL_STACK_H
#define VAL_STACK_H


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
        size_t store_idx;
    } data;
} __attribute__((__packed__)) StkVal;

void createValStack();
void destroyValStack();
int getValStackLen();
StkVal initVal(uint16_t type, void* val);
void pushVal(StkVal val);
StkVal popVal();
StkVal peekVal(size_t offset);
StkVal peekStk();
void printVal(int level, uint8_t type, void* val);
void assignVal(int index, StkVal val);

#endif