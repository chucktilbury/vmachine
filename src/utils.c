
#include "common.h"

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
