#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>
#include <stdio.h>
#include "object.h"

typedef struct _str_obj_ {
    const char* str;
    size_t len;
    size_t obj_index; // for saving and loading only
    struct _str_obj_* left;
    struct _str_obj_* right;
} StringObj;

size_t addStr(const char* str);
void deleteStrObj(size_t index);
const char* getStrObj(size_t index);
size_t getStrObjLen(size_t index);
bool compareStrObj(size_t index, const char* str);
size_t addStrObj(size_t left, size_t right);

void saveStrObj(StringObj* obj, FILE* fp);
StringObj* loadStrObj(FILE* fp);

#endif