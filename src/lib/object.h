#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdio.h>

typedef enum {
    OBJ_ERROR,
    OBJ_STRING,
    OBJ_USR,
} ObjType;

typedef struct {
    ObjType type;
    void* obj;
} Object;

void createObjStore();
void destroyObjStore();

void initObj(Object* obj, ObjType type, void* data);
int addObj(Object obj);
Object getObj(size_t index);

const char* objTypeToStr(ObjType type);

void saveObjStore(FILE* fp);
void loadObjStore(FILE* fp);

#endif