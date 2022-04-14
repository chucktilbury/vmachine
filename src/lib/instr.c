
#include "common.h"

typedef struct {
    uint8_t* list;
    size_t cap;
    size_t len;
    size_t index;
} InstStore;

static InstStore store;

static inline void grow_list(size_t size)
{
    if(store.cap < store.len + size) {
        while(store.cap < store.len + size) {
            store.cap <<= 1;
        }
        store.list = _realloc_ds_array(store.list, uint8_t, store.cap);
    }
    // else do nothing
}

void createInstStore()
{
    store.cap = 0x01 << 3;
    store.len = 0;
    store.index = 0;
    store.list = _alloc_ds_array(uint8_t, store.cap);
}

void destroyInstStore()
{
    if(store.list != NULL) {
        _free(store.list);
    }
}

void write8(uint8_t val)
{
    grow_list(1);
    *((uint8_t*)&store.list[store.len]) = val;
    store.len += 1;
}

void write16(uint16_t val)
{
    grow_list(2);
    *((uint16_t*)&store.list[store.len]) = val;
    store.len += 2;
}

void write32(uint32_t val)
{
    grow_list(4);
    *((uint32_t*)&store.list[store.len]) = val;
    store.len += 4;
}

uint8_t read8()
{
    uint8_t val = *((uint8_t*)&store.list[store.index]);
    store.index += 1;
    return val;
}

uint16_t read16()
{
    uint16_t val = *((uint16_t*)&store.list[store.index]);
    store.index += 2;
    return val;
}

uint32_t read32()
{
    uint32_t val = *((uint32_t*)&store.list[store.index]);
    store.index += 4;
    return val;
}

int getIndex()
{
    return store.index;
}

int getLen()
{
    return store.len;
}

int setIndex(int idx)
{
    return (store.index = idx);
}

int addIndex(int idx)
{
    // idx could be negative
    return (store.index += idx);
}

void saveInstStore(FILE* fp)
{
    fwrite(&store.len, sizeof(size_t), 1, fp);
    fwrite(store.list, sizeof(char), store.len, fp);
}

void loadInstStore(FILE* fp)
{
    memset(&store, 0, sizeof(InstStore));
    fread(&store.len, sizeof(size_t), 1, fp);

    store.cap = 1;
    while((store.len + 1) > store.cap) {
        store.cap <<= 1;
    }

    store.list = _alloc_ds_array(uint8_t, store.cap);

    fread(store.list, sizeof(uint8_t), store.len, fp);
    //printf("len: %lu cap: %lu list: %p\n", store.len, store.cap, store.list);
}