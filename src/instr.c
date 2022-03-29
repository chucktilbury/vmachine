
#include "common.h"
#include "instr.h"

static inline void grow_list(InstStore* is, size_t size)
{
    if(is->cap < is->len + size) {
        while(is->cap < is->len + size)
            is->cap <<= 1;
        is->list = realloc(is->list, is->cap);
        assert(is->list != NULL);
    }
    // else do nothing
}

InstStore* createInstStore()
{
    InstStore* is = malloc(sizeof(InstStore));
    assert(is != NULL);

    is->cap = 0x01 << 3;
    is->len = 0;
    is->index = 0;
    is->list = malloc(is->cap);
    assert(is->list != NULL);

    return is;
}

void destroyInstStore(InstStore* is)
{
    if(is != NULL) {
        if(is->list != NULL)
            free(is->list);
        free(is);
    }
}

void write8InstStore(InstStore* is, uint8_t val)
{
    grow_list(is, 1);
    *((uint8_t*)&is->list[is->len]) = val;
    is->len += 1;
}

void write16InstStore(InstStore* is, uint16_t val)
{
    grow_list(is, 2);
    *((uint16_t*)&is->list[is->len]) = val;
    is->len += 2;
}

void write32InstStore(InstStore* is, uint32_t val)
{
    grow_list(is, 4);
    *((uint32_t*)&is->list[is->len]) = val;
    is->len += 4;
}

void write64InstStore(InstStore* is, uint64_t val)
{
    grow_list(is, 8);
    *((uint32_t*)&is->list[is->len]) = val;
    is->len += 8;
}

uint8_t read8InstStore(InstStore* is)
{
    uint8_t val = *((uint8_t*)&is->list[is->index]);
    is->index += 1;
    return val;
}

uint16_t read16InstStore(InstStore* is)
{
    uint16_t val = *((uint16_t*)&is->list[is->index]);
    is->index += 2;
    return val;
}

uint32_t read32InstStore(InstStore* is)
{
    uint32_t val = *((uint32_t*)&is->list[is->index]);
    is->index += 4;
    return val;
}

uint64_t read64InstStore(InstStore* is)
{
    uint64_t val = *((uint64_t*)&is->list[is->index]);
    is->index += 8;
    return val;
}

int getIndex(InstStore* is)
{
    return is->index;
}

int setIndex(InstStore* is, int idx)
{
    return (is->index = idx);
}

int addIndex(InstStore* is, int idx)
{   // idx could be negative
    return (is->index += idx);
}
