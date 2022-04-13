#ifndef _FILE_IO_H_
#define _FILE_IO_H_

// #include <stdio.h>
// #include <stdint.h>

// void writeObjStore(FILE* fp, VarStore* os);
// void readObjStore(FILE* fp, VarStore* os);
//
// void writeInstStore(FILE* fp, InstStore* is);
// void readInstStore(FILE* fp, InstStore* is);
//
// void writeStrStore(FILE* fp, StrStore* ss);
// void readStrStore(FILE* fp, StrStore* ss);

void loadVM(const char* fname);
void saveVM(const char* fname);

#endif