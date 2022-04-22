#ifndef OBJECT_H
#define OBJECT_H

typedef enum {
    OBJ_STRING,
    OBJ_USR,
} ObjType;

typedef struct {
    ObjType type;
    void* obj;
} Object;

void createObjStore();
void destroyObjStore();

void initObj(Object* obj, ObjType type);
int addObj(Object obj);
Object getObj(int index);

#endif