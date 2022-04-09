
#include "common.h"
#include "ptr_list.h"

GenericPtrList* createGPL()
{
    GenericPtrList* gpl = _alloc_ds(GenericPtrList);
    gpl->cap = 0x01 << 3;
    gpl->len = 0;
    gpl->idx = 0;
    gpl->list = _alloc_ds_array(void*, gpl->cap);

    return gpl;
}

void destroyGPL(GenericPtrList* gpl)
{
    if(gpl != NULL) {
        if(gpl->list != NULL) {
            _free(gpl->list);
        }
        _free(gpl);
    }
}

int addGPL(GenericPtrList* gpl, void* data)
{
    int slot = -1;

    for(int i = 0; i < gpl->len; i++) {
        if(gpl->list[i] == NULL) {
            slot = i;
            break;
        }
    }

    if(slot < 0) {
        if(gpl->len + 1 > gpl->cap) {
            gpl->cap <<= 1;
            gpl->list = _realloc_ds_array(gpl->list, void*, gpl->cap);
        }
        slot = gpl->len;
        gpl->len++;
    }

    gpl->list[slot] = data;

    return slot;
}

void* getGPL(GenericPtrList* gpl, int idx)
{
    if(idx < gpl->len) {
        return gpl->list[idx];
    }

    return NULL;
}

void setGPL(GenericPtrList* gpl, int idx, void* data)
{
    if(idx < gpl->len) {
        gpl->list[idx] = data;
    }
}

void* delGPL(GenericPtrList* gpl, int idx)
{
    void* retv = NULL;

    if(idx < gpl->len) {
        retv = gpl->list[idx];
        gpl->list[idx] = NULL; // the caller has to free this
    }

    return retv;
}

void resetGPL(GenericPtrList* gpl)
{
    gpl->idx = 0;
}

void* iterGPL(GenericPtrList* gpl)
{
    return (gpl->idx < gpl->len) ? gpl->list[gpl->idx++] : NULL;
}

int pushGPL(GenericPtrList* gpl, void* data)
{
    if(gpl->len + 1 > gpl->cap) {
        gpl->cap <<= 1;
        gpl->list = _realloc_ds_array(gpl->list, void*, gpl->cap);
    }

    gpl->list[gpl->len] = data;
    gpl->len++;

    return gpl->len - 1;
}

void* popGPL(GenericPtrList* gpl)
{
    gpl->len--;
    if(gpl->len < 0) {
        gpl->len = 0;
        return NULL;
    }

    void* retv = gpl->list[gpl->len];
    gpl->list[gpl->len] = NULL;

    return retv;
}

void* peekGPL(GenericPtrList* gpl)
{
    if(gpl->len - 1 < 0) {
        return NULL;
    }
    return gpl->list[gpl->len - 1];
}

