#ifndef _STRINGS_H_
#define _STRINGS_H_

#include "vm_support.h"

typedef struct _obj_str_ {
    uint16_t len;
    const char* str;
    StoreIndex idx;
    struct _obj_str_* left;
    struct _obj_str_* right;
    struct _obj_str_* back;
} ObjString;

/**
 * Store strings in 2 ways for 2 different perspectives. The first, in the
 * root, is as a binary tree. This is used to find the slot number of a given
 * string for the instruction stream and to prevent storing multiple copes of
 * the same string. The string list is for looking up a string by its slot
 * number. The slot number is what is stored as immediate values in the
 * instruction stream.
 */
typedef struct {
    ObjString* root;
    ObjString** list;
    size_t cap;
    size_t len;
} StrStore;

StrStore* createStrStore();
void destroyStrStore(StrStore* ss);
void destroyStrNode(ObjString* obj);

StoreIndex addStrStore(StrStore* ss, const char* str);
const char* getStrStoreStr(StrStore* ss, StoreIndex idx);
StoreIndex getStrStoreSlot(StrStore* ss, const char* str);
void delStrStore(StrStore* ss, StoreIndex idx);

#endif