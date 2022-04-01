
#include "common.h"
#include "vm_support.h"

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
    obj->type = VAL_ERROR;
    obj->isAssigned = true;
    obj->data.unum = 0;
}

ValueStore* createValStore()
{
    ValueStore* obj = _alloc_ds(ValueStore);
    assert(obj != NULL);

    obj->cap = 0x01 << 3;
    obj->len = 0;
    obj->list = _alloc_ds_array(Value, obj->cap);

    Value eobj;
    init_error_obj(&eobj);
    obj->list[0] = eobj;
    obj->len = 1;

    return obj;
}

void destroyValStore(ValueStore* os)
{
    if(os != NULL) {
        if(os->list != NULL)
            _free(os->list);
        _free(os);
    }
}

void initVal(Value* obj, ValueType type)
{
    obj->type = type;
    obj->isAssigned = false;
}

void pushVal(ValueStore* os, Value obj)
{
    addVal(os, obj);
}

Value popVal(ValueStore* os)
{
    assert(os->len > 0);
    os->len--;
    return os->list[os->len];
}

Value peekVal(ValueStore* os)
{
    assert(os->len > 0);
    return os->list[os->len-1];
}

Index addVal(ValueStore* os, Value obj)
{
    grow_store(os);
    os->list[os->len] = obj;
    os->len ++;

    return os->len-1;
}

Value getVal(ValueStore* os, Index index)
{
    if(index < os->len)
        return os->list[index];
    else
        return os->list[0];
}

Value setVal(ValueStore* os, Index index, Value obj)
{
    if(index < os->len) {
        os->list[index] = obj;
        return obj;
    }
    else
        return os->list[0];
}

void assignVal(Value* obj, ValueType type, void* data)
{
    switch(type) {
        case VAL_STRING:
        case VAL_ERROR:
            obj->data.obj = *(Index*)data;
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
        case VAL_ADDRESS:
            obj->data.unum = *((uint64_t*)data);
            break;
        default:
            runtimeError("attempt to assign an unknown object type: %d", type);
    }
    obj->type = type;
}

const char* valToStr(ValueType type)
{
    return (type == VAL_ERROR)? "VAL_ERROR":
            (type == VAL_UNUM)? "VAL_UNUM":
            (type == VAL_INUM)? "VAL_INUM":
            (type == VAL_FNUM)? "VAL_FNUM":
            (type == VAL_BOOL)? "VAL_BOOL":
            (type == VAL_STRING)? "VAL_STRING":
            (type == VAL_ADDRESS)? "VAL_ADDRESS": "UNKNOWN";
}

