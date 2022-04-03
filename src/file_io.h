#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <stdio.h>
#include <stdint.h>

void writeObjStore(FILE* fp, ValueStore* os);
void readObjStore(FILE* fp, ValueStore* os);

void writeInstStore(FILE* fp, InstStore* is);
void readInstStore(FILE* fp, InstStore* is);

void writeStrStore(FILE* fp, StrStore* ss);
void readStrStore(FILE* fp, StrStore* ss);

VMachine* loadVM(const char* fname);
void saveVM(VMachine* vm, const char* fname);

#endif