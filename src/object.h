#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdint.h>
#include <stddef.h>

typedef enum {
    OBJ_ERROR,
    OBJ_UNUM,
    OBJ_INUM,
    OBJ_FNUM,
    OBJ_BOOL,
    OBJ_STRING,
    OBJ_INDEX,
} ObjectType;

typedef struct {
    ObjectType type;
    const char* name;
    bool isAssigned;
    bool isWritable;
    bool inUse;
    union {
        uint64_t unum;
        int64_t inum;
        double fnum;
        bool boolean;
        int index;
    } data;
} Object;

typedef struct {
    Object* list;
    int cap;
    int len;
} ObjStore;

ObjStore* createObjStore();
void destroyObjStore(ObjStore* os);

void initObj(Object* obj, ObjectType type);
int addObjStore(ObjStore* os, Object obj);
Object getObjStore(ObjStore* os, int index);
Object setObjStore(ObjStore* os, int index, Object obj);

void pushObjStore(ObjStore* os, Object obj);
Object popObjStore(ObjStore* os);
Object peekObjStore(ObjStore* os);

#endif