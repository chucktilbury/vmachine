#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <stdio.h>
#include <stdint.h>
#include "vm_support.h"

void writeObjStore(FILE* fp, ValueStore* os);
void readObjStore(FILE* fp, ValueStore* os);

void writeInstStore(FILE* fp, InstStore* is);
void readInstStore(FILE* fp, InstStore* is);

VirtualMachine* loadVM(const char* fname);
void saveVM(const char* fname, VirtualMachine* vm);

#endif