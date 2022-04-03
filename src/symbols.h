#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#include <stddef.h>
#include "vm_support.h"

typedef struct _sym_ {
    const char* key;
    size_t len;
    Index idx; // the value that this symbol references
    struct _sym_* left;
    struct _sym_* right;
} Symbol;

void destroySymbols();

void addSymbol(const char* key, Index idx);
Index findSymbol(const char* key);

void dumpSymbols(VMachine* vm);

#endif