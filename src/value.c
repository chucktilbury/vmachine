
#include "common.h"
#include "value.h"
#include "error.h"

// TODO: reuse deleted slots.
static inline void grow_store(ValueStore* os)
{
    if(os->cap < os->len+1) {
        os->cap <<= 1;
        os->list = realloc(os->list, sizeof(ValueStore)*os->cap);
        assert(os->list != NULL);
    }
    // else do nothing
}

static void init_error_obj(Value* obj)
{
    obj->inUse = true;
    obj->type = VAL_ERROR;
    obj->name = "ERROR";
    obj->isAssigned = true;
    obj->isWritable = false;
}

ValueStore* createObjStore()
{
    ValueStore* obj = malloc(sizeof(ValueStore));
    assert(obj != NULL);

    obj->cap = 0x01 << 3;
    obj->len = 0;
    obj->list = malloc(sizeof(ValueStore)*obj->cap);
    assert(obj->list != NULL);

    Value eobj;
    init_error_obj(&eobj);
    obj->list[0] = eobj;
    obj->len = 1;

    return obj;
}

void destroyObjStore(ValueStore* os)
{
    if(os != NULL) {
        if(os->list != NULL)
            free(os->list);
        free(os);
    }
}

void initObj(Value* obj, ValueType type)
{
    obj->type = type;
    obj->name = NULL;
    obj->isAssigned = false;
    obj->isWritable = true;
    obj->inUse = true;
}

void pushObjStore(ValueStore* os, Value obj)
{
    addObjStore(os, obj);
}

Value popObjStore(ValueStore* os)
{
    assert(os->len > 0);
    os->len--;
    return os->list[os->len];
}

Value peekObjStore(ValueStore* os)
{
    assert(os->len > 0);
    return os->list[os->len-1];
}

int addObjStore(ValueStore* os, Value obj)
{
    grow_store(os);
    os->list[os->len] = obj;
    os->len ++;

    return os->len-1;
}

Value getObjStore(ValueStore* os, int index)
{
    Value obj;

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

Value setObjStore(ValueStore* os, int index, Value obj)
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

void assignObj(Value* obj, ValueType type, void* data)
{
    switch(type) {
        case VAL_ERROR:
            obj->data.str = (char*)data;
            break;
        case VAL_UNUM:
            obj->data.unum = *((uint64_t*)data);
            break;
        case VAL_INUM:
            obj->data.inum = *((int64_t*)data);
            break;
        case VAL_FNUM:
            obj->data.fnum = *((double*)data);
            break;
        case VAL_BOOL:
            obj->data.boolean = *((bool*)data);
            break;
        case VAL_STRING:
            obj->data.str = (char*)data;
            break;
        case VAL_INDEX:
            obj->data.unum = *((uint64_t*)data);
            break;
        default:
            runtimeError("attempt to assign an unknown object type: %d", type);
    }
    obj->type = type;
}

const char* objToStr(ValueType type)
{
    return (type == VAL_ERROR)? "VAL_ERROR":
            (type == VAL_UNUM)? "VAL_UNUM":
            (type == VAL_INUM)? "VAL_INUM":
            (type == VAL_FNUM)? "VAL_FNUM":
            (type == VAL_BOOL)? "VAL_BOOL":
            (type == VAL_STRING)? "VAL_STRING":
            (type == VAL_INDEX)? "VAL_INDEX": "UNKNOWN";
}

void printValue(Value obj)
{
    printf("%s: ", objToStr(obj.type));
    switch(obj.type) {
        case VAL_STRING:
        case VAL_ERROR:
            printf("%s\n", obj.data.str);
            break;
        case VAL_UNUM:
            printf("0x%08lX\n", obj.data.unum);
            break;
        case VAL_INUM:
            printf("%ld\n", obj.data.inum);
            break;
        case VAL_FNUM:
            printf("%0.3f\n", obj.data.fnum);
            break;
        case VAL_BOOL:
            printf("%s\n", obj.data.boolean? "TRUE": "FALSE");
            break;
        case VAL_INDEX:
            printf("not supported\n");
            break;
        default:
            printf("object value not found");
            break;
    }
}

