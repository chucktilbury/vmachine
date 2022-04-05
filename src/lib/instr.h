#ifndef _INSTR_H_
#define _INSTR_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t* list;
    size_t cap;
    size_t len;
    size_t index;
} InstStore;

InstStore* createInstStore();
void destroyInstStore(InstStore* is);

void write8InstStore(InstStore* is, uint8_t val);
void write16InstStore(InstStore* is, uint16_t val);
void write32InstStore(InstStore* is, uint32_t val);
void write64InstStore(InstStore* is, uint64_t val);

uint8_t read8InstStore(InstStore* is);
uint16_t read16InstStore(InstStore* is);
uint32_t read32InstStore(InstStore* is);
uint64_t read64InstStore(InstStore* is);

int getIndex(InstStore* is);
int setIndex(InstStore* is, int idx);
int addIndex(InstStore* is, int idx);

#endif