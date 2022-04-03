
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

void printVal(VMachine* vm, Value obj)
{
    printf("%-12s", valToStr(obj.type));
    int pad_len = 10;
    switch(obj.type) {
        case VAL_ERROR: {
                int slen = snprintf(NULL, 0, "ERROR");
                printf("ERROR%*s", pad_len - slen, " ");
            }
            break;
        case VAL_STRING: {
                int slen = snprintf(NULL, 0, "%s", getStr(vm->str_store, obj.data.obj));
                printf("%s%*s", getStr(vm->str_store, obj.data.obj), pad_len - slen, " ");
            }
            break;
        case VAL_UNUM: {
                int slen = snprintf(NULL, 0, "0x%lX", obj.data.unum);
                printf("0x%lX%*s", obj.data.unum, pad_len - slen, " ");
            }
            break;
        case VAL_ADDRESS:
        case VAL_INUM: {
                int slen = snprintf(NULL, 0, "%ld", obj.data.inum);
                printf("%ld%*s", obj.data.inum, pad_len - slen, " ");
            }
            break;
        case VAL_FNUM: {
                int slen = snprintf(NULL, 0, "%0.1f", obj.data.fnum);
                printf("%0.1f%*s", obj.data.fnum, pad_len - slen, " ");
            }
            break;
        case VAL_BOOL: {
                int slen = snprintf(NULL, 0, "%s", obj.data.boolean? "TRUE": "FALSE");
                printf("%s%*s", obj.data.boolean? "TRUE": "FALSE", pad_len - slen, " ");
            }
            break;
        default:
            printf("object value not found");
            break;
    }

    printf("assigned: %s const: %s\n", obj.isAssigned? "true": "false", obj.isConst? "true": "false");
}

