
#include "common.h"
#include "object.h"

// TODO: reuse deleted slots.
static inline void grow_store(ObjStore* os)
{
    if(os->cap < os->len+1) {
        os->cap <<= 1;
        os->list = realloc(os->list, sizeof(ObjStore)*os->cap);
        assert(os->list != NULL);
    }
    // else do nothing
}

static void init_error_obj(Object* obj)
{
    obj->inUse = true;
    obj->type = OBJ_ERROR;
    obj->name = "ERROR";
    obj->isAssigned = true;
    obj->isWritable = false;
}

ObjStore* createObjStore()
{
    ObjStore* obj = malloc(sizeof(ObjStore));
    assert(obj != NULL);

    obj->cap = 0x01 << 3;
    obj->len = 0;
    obj->list = malloc(sizeof(ObjStore)*obj->cap);
    assert(obj->list != NULL);

    Object eobj;
    init_error_obj(&eobj);
    obj->list[0] = eobj;
    obj->len = 1;

    return obj;
}

void destroyObjStore(ObjStore* os)
{
    if(os != NULL) {
        if(os->list != NULL)
            free(os->list);
        free(os);
    }
}

void initObj(Object* obj, ObjectType type)
{
    obj->type = type;
    obj->name = NULL;
    obj->isAssigned = false;
    obj->isWritable = true;
    obj->inUse = true;
}

void pushObjStore(ObjStore* os, Object obj)
{
    addObjStore(os, obj);
}

Object popObjStore(ObjStore* os)
{
    assert(os->len > 0);
    os->len--;
    return os->list[os->len];
}

Object peekObjStore(ObjStore* os)
{
    assert(os->len > 0);
    return os->list[os->len-1];
}

int addObjStore(ObjStore* os, Object obj)
{
    grow_store(os);
    os->list[os->len] = obj;
    os->len ++;

    return os->len-1;
}

Object getObjStore(ObjStore* os, int index)
{
    Object obj;

    if(index >= 0 && index < os->len) {
        obj = os->list[index];
        if(obj.inUse)
            return obj;
        else
            return os->list[0];
    }
    else
        return os->list[0];
}

Object setObjStore(ObjStore* os, int index, Object obj)
{
    if(index >= 0 && index < os->len) {
        obj = os->list[index];
        if(obj.isWritable) {
            os->list[index] = obj;
            return obj;
        }
        else
            return os->list[0];
    }
    else
        return os->list[0];
}
