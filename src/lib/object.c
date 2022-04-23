/**
 * @file object.c
 * @brief Generic objects
 *
 * @version 0.1
 * @date 2022-04-22
 *
 */
#include "common.h"
#include "vmachine.h"

typedef struct {
    Object* list;
    size_t cap;
    size_t len;
} ObjStore;

static ObjStore obj_store;

void createObjStore()
{
    obj_store.cap = 0x01 << 3;
    obj_store.len = 0;
    obj_store.list = _alloc_ds_array(Object, obj_store.cap);

    initObj(&obj_store.list[0], OBJ_ERROR, NULL);
    obj_store.len++;
}

void destroyObjStore()
{
    if(obj_store.list != NULL) {
        _free(obj_store.list);
        obj_store.list = NULL;
    }
}

void initObj(Object* obj, ObjType type, void* data)
{
    obj->type = type;
    obj->obj = data;
}

int addObj(Object obj)
{
    if((obj_store.len+1) > obj_store.cap) {
        obj_store.cap <<= 1;
        obj_store.list = _realloc_ds_array(obj_store.list, Object, obj_store.cap);
    }

    obj_store.list[obj_store.len] = obj;
    obj_store.len++;

    return obj_store.len-1;
}

Object getObj(size_t index)
{
    if(index >= obj_store.len) {
        genericError("invalid object store index: %d", index);
        return obj_store.list[0];
    }

    return obj_store.list[index];
}

void saveObjStore(FILE* fp)
{
    (void)fp;
}

void loadObjStore(FILE* fp)
{
    (void)fp;
}

const char* objTypeToStr(ObjType type)
{
    return (type == OBJ_ERROR)? "ERROR":
        (type == OBJ_STRING)? "STRING":
        (type == OBJ_USR)? "USER": "UNKNOWN";
}
