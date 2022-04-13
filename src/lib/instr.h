#ifndef _INSTR_H_
#define _INSTR_H_

#include <stdint.h>
#include <stddef.h>

void createInstStore();
void destroyInstStore();

void write8(uint8_t val);
void write16(uint16_t val);
void write32(uint32_t val);

uint8_t read8();
uint16_t read16();
uint32_t read32();

int getIndex();
int getLen();
int setIndex(int idx);
int addIndex(int idx);

void saveInstStore(FILE* fp);
void loadInstStore(FILE* fp);

#endif