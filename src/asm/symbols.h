#ifndef _SYMBOLS_H_
#define _SYMBOLS_H_

#include <stddef.h>
#include <stdint.h>
#include "vmachine.h"

typedef struct _sym_ {
    const char* key;
    size_t len;
    uint16_t idx; // the value that this symbol references
    int line;
    int col;
    const char* filename;
    struct _sym_* left;
    struct _sym_* right;
} Symbol;

void destroySymbols();

void addSymbol(const char* key, uint16_t idx);
uint16_t findSymbol(const char* key);

void dumpSymbols();

#endif