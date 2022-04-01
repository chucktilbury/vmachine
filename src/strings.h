#ifndef _STRINGS_H_
#define _STRINGS_H_

#include "vm_support.h"

typedef struct {
    uint16_t len;
    const char* str;
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
    ObjString* list;
    size_t cap;
    size_t len;
} StrStore;

StrStore* createStrStore();
void destroyStrStore(StrStore* ss);

Index addStr(StrStore* ss, const char* str);
const char* getStr(StrStore* ss, Index idx);
void delStr(StrStore* ss, Index idx);

ObjString* getStrByIndex(StrStore* ss, Index idx);
ObjString* getStrByStr(StrStore* ss, const char* str);

int add_tree(ObjString* tree, ObjString*node);

#endif