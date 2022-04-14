
#include "common.h"
#include "call_stack.h"
#include "memory.h"

static CallStack* call_stack;

void createCallStack()
{
    call_stack = _alloc_ds(CallStack);
    call_stack->cap = 0x01 << 8;
    call_stack->len = 0;
    call_stack->list = _alloc_ds_array(CallElem, call_stack->cap);
}

void destroyCallStack()
{
    if(call_stack != NULL) {
        if(call_stack->list != NULL) {
            _free(call_stack->list);
        }
        _free(call_stack);
    }
}

CallElem initCallElem(uint32_t ret, uint32_t base)
{
    CallElem elem;

    elem.ret_addr = ret;
    elem.base_ptr = base;
    //elem.nparm = nparm;

    return elem;
}

void pushCall(CallElem ce)
{
    if((call_stack->len + 1) > call_stack->cap) {
        call_stack->cap <<= 1;
        call_stack->list = _realloc_ds_array(call_stack->list, CallElem, call_stack->cap);
    }

    call_stack->list[call_stack->len] = ce;
    call_stack->len++;
}

CallElem popCall()
{
    if((call_stack->len - 1) > call_stack->len) {
        fprintf(stderr, "Fatal Error: call pop stack under flow\n");
        exit(1);
    }

    call_stack->len--;
    return call_stack->list[call_stack->len];
}

CallElem peekCall()
{
    if((call_stack->len - 1) > call_stack->len) {
        fprintf(stderr, "Fatal Error: call peek stack under flow\n");
        exit(1);
    }

    return call_stack->list[call_stack->len - 1];
}

uint32_t peekCallBase()
{
    if((call_stack->len - 1) > call_stack->len) {
        fprintf(stderr, "Fatal Error: call peek stack under flow\n");
        exit(1);
    }

    return call_stack->list[call_stack->len - 1].base_ptr;
}

uint32_t peekCallRet()
{
    if((call_stack->len - 1) > call_stack->len) {
        fprintf(stderr, "Fatal Error: call peek stack under flow\n");
        exit(1);
    }

    return call_stack->list[call_stack->len - 1].ret_addr;
}
