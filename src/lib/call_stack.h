#ifndef _CALL_STACK_H_
#define _CALL_STACK_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t ret_addr;
    uint32_t base_ptr;
    //uint16_t base_ptr;
    //uint8_t nparm;
} __attribute__((__packed__)) CallElem;

typedef struct {
    CallElem* list;
    size_t cap;
    size_t len;
} CallStack;

void createCallStack();
void destroyCallStack();

CallElem initCallElem(uint32_t ret, uint32_t base);
void pushCall(CallElem ce);
CallElem popCall();
CallElem peekCall();
uint32_t peekCallBase();
uint32_t peekCallRet();

#endif