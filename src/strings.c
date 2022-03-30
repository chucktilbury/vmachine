
#include "common.h"
#include "vm_support.h"

static StoreIndex find_slot(StrStore* ss)
{
    for(StoreIndex i = 0; i < ss->len; i++) {
        if(ss->list[i] == NULL)
            return i;
    }

    // not reusing a free()d slot
    if(ss->cap < ss->len+1) {
        ss->cap <<= 1;
        ss->list = _realloc_ds_array(ss->list, ObjString*, ss->cap);
    }

    return (StoreIndex)ss->len;
}

StrStore* createStrStore()
{
    StrStore* ss = _alloc_ds(StrStore);

    ss->cap = 0x01 << 3;
    ss->len = 0;
    ss->root = NULL;
    ss->list = _alloc_ds_array(ObjString*, ss->cap);

    return ss;
}

void destroyStrNode(ObjString* obj)
{
    if(obj != NULL) {
        _free((void*)obj->str);
        _free(obj);
    }
}

void destroyStrStore(StrStore* ss)
{
    if(ss != NULL) {
        if(ss->list != NULL)
            for(StoreIndex i = 0; i < ss->len; i++) {
                if(ss->list[i] != NULL) {
                    destroyStrNode(ss->list[i]);
                }
            }
            _free(ss->list);
        _free(ss);
    }
}

static int add_tree(ObjString* tree, ObjString*node)
{
    int val = strcmp(tree->str, node->str);
    if(val > 0) {
        if(tree->right != NULL)
            return add_tree(tree->right, node);
        else {
            tree->right = node;
            node->back = tree;
            return 1;
        }
    }
    else if(val < 0) {
        if(tree->left != NULL)
            return add_tree(tree->left, node);
        else {
            tree->left = node;
            node->back = tree;
            return 1;
        }
    }
    else
        return 0;   // node already exists
}

StoreIndex addStrStore(StrStore* ss, const char* str)
{
    ObjString* obj = _alloc_ds(ObjString);
    obj->len = strlen(str);
    obj->str = strdup(str);
    obj->idx = 0;
    obj->left = NULL;
    obj->right = NULL;

    StoreIndex slot;
    if(add_tree(ss->root, obj)) {
        slot = find_slot(ss);
        obj->idx = slot;
        ss->list[slot] = obj;
        if(slot == ss->len)
            ss->len++;
    }
    else {
        _free((void*)obj->str);
        _free(obj);
    }

    return slot;
}

const char* getStrStoreStr(StrStore* ss, StoreIndex idx)
{
    if(idx < ss->len && ss->list[idx] != NULL) {
        return ss->list[idx]->str;
    }

    return NULL;
}

static StoreIndex find_tree(ObjString* tree, const char* str)
{
    int val = strcmp(tree->str, str);
    if(val > 0) {
        if(tree->right != NULL)
            return find_tree(tree->right, str);
        else
            return 0;   // not found
    }
    else if(val < 0) {
        if(tree->left != NULL)
            return find_tree(tree->left, str);
        else
            return 0;   // not found
    }
    else
        return tree->idx;
}

StoreIndex getStrStoreSlot(StrStore* ss, const char* str)
{
    return find_tree(ss->root, str);
}

void delStrStore(StrStore* ss, StoreIndex idx)
{
    if(idx < ss->len && ss->list[idx] != NULL) {
        ObjString* me = ss->list[idx];
        ObjString* left = me->left;
        ObjString* right = me->right;
        ObjString* back = me->back;

        if(me == back->left)
            back->left = NULL;
        else if(me == back->right)
            back->right = NULL;
        else {
            fprintf(stderr, "fatal error: string tree messed up\n");
            exit(1);
        }

        if(left != NULL)
            add_tree(ss->root, left);
        if(right != NULL)
            add_tree(ss->root, right);

        destroyStrNode(me);
        ss->list[idx] = NULL;
    }
    // else fail silently
}
