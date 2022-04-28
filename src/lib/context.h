#ifndef CONTEXT_H
#define CONTEXT_H

#include "str_store.h"
#include "var_store.h"
#include "instr.h"
#include "val_stack.h"

typedef enum {
    CON_ERROR,
    CON_BLOCK,
    CON_NAME,
    CON_STRUCT,
} ConType;

typedef struct _context_ {

    const char* key;
    ConType type;

    InstStore* instr;
    ValStack* val_stk;
    VarStore* vars;
    StrStore* strs;

    struct _context_* left;
    struct _context_* right;
    struct _context_* parent;
    struct _context_* children;
} Context;

typedef struct _con_store_ {
    Context** list;
    size_t cap;
    size_t len;
} ConStack;

Context* createContext(const char* name);
void destroyContext(Context* ctx);

void saveConStore(FILE* fp);
void loadConStore(FILE* fp);

#endif