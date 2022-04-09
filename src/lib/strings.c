
#include "common.h"

// FIXME: Do not store duplicate strings
static Index find_slot(StrStore* ss, const char* str)
{
    (void)str;

    for(Index i = 0; i < ss->len; i++) {
        if(ss->list[i].len == 0) {
            return i;
        }
    }

    // not reusing a free()d slot
    if(ss->cap < ss->len + 1) {
        ss->cap <<= 1;
        ss->list = _realloc_ds_array(ss->list, ObjString, ss->cap);
    }

    return (Index)ss->len;
}

StrStore* createStrStore()
{
    StrStore* ss = _alloc_ds(StrStore);

    ss->cap = 0x01 << 3;
    ss->len = 0;
    ss->list = _alloc_ds_array(ObjString, ss->cap);

    return ss;
}

void destroyStrStore(StrStore* ss)
{
    if(ss != NULL) {
        if(ss->list != NULL)
            for(Index i = 0; i < ss->len; i++) {
                if(ss->list[i].len != 0) {
                    _free((void*)ss->list[i].str);
                }
            }
        _free(ss->list);
        _free(ss);
    }
}

Index addStr(StrStore* ss, const char* str)
{
    Index slot = find_slot(ss, str);
    ss->list[slot].len = strlen(str) + 1;
    ss->list[slot].str = _strdup(str);

    return slot;
}

const char* getStr(StrStore* ss, Index idx)
{
    if(idx < ss->len && ss->list[idx].len != 0) {
        return ss->list[idx].str;
    }

    return NULL;
}

void delStr(StrStore* ss, Index idx)
{
    if(idx < ss->len && ss->list[idx].len != 0) {
        _free((void*)ss->list[idx].str);
        ss->list[idx].len = 0;
    }
    // else fail silently
}
