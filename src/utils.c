
#include "common.h"
#include "vm_support.h"

#ifndef strdup
const char* strdup(const char* str)
{
    if(str != NULL) {
        size_t len = strlen(str) + 1;
        char* s = malloc(len);
        memcpy(s, str, len);
        return s;
    }
    else {
        char* s = malloc(1);
        s[0] = '\0';
        return s;
    }
}
#endif

void* memalloc(size_t size)
{
    void* ptr = malloc(size);
    assert(ptr != NULL);
    memset(ptr, 0, size);
    return ptr;
}

void* memrealloc(void* ptr, size_t size)
{
    void* nptr = realloc(ptr, size);
    assert(nptr != NULL);
    return nptr;
}

void memfree(void* ptr)
{
    if(ptr != NULL)
        free(ptr);
}

void printValue(VirtualMachine* vm, Value obj)
{
    printf("%s: ", valToStr(obj.type));
    switch(obj.type) {
        case VAL_ERROR:
            printf("ERROR\n");
            break;
        case VAL_STRING:
            printf("%s\n", getStr(vm->str_store, obj.data.obj));
            break;
        case VAL_UNUM:
            printf("0x%08lX\n", obj.data.unum);
            break;
        case VAL_INUM:
            printf("%ld\n", obj.data.inum);
            break;
        case VAL_FNUM:
            printf("%0.3f\n", obj.data.fnum);
            break;
        case VAL_BOOL:
            printf("%s\n", obj.data.boolean? "TRUE": "FALSE");
            break;
        case VAL_ADDRESS:
            printf("not supported\n");
            break;
        default:
            printf("object value not found");
            break;
    }
}

