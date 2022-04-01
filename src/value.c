
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
    obj->isConst = true;
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
    obj->isConst = false;
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

void assignVal(Value* obj, Value* val)
{
    switch(obj->type) {
        case VAL_STRING: {
            switch(obj->type) {
                case VAL_STRING:
                case VAL_ERROR:
                    obj->data.obj = val->data.obj;
                    break;
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_ERROR:
            switch(obj->type) {
                case VAL_STRING:
                case VAL_ERROR:
                    obj->data.obj = val->data.obj;
                    break;
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_UNUM:
            switch(obj->type) {
                case VAL_UNUM:  obj->data.unum = val->data.unum; break;
                case VAL_INUM:  obj->data.unum = (uint64_t)val->data.inum; break;
                case VAL_FNUM:  obj->data.unum = (uint64_t)((int64_t)val->data.fnum); break;
                case VAL_ADDRESS:  obj->data.unum = val->data.unum; break;
                // errors
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_ERROR:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_INUM:
            switch(obj->type) {
                case VAL_UNUM:  obj->data.inum = (int64_t)val->data.unum; break;
                case VAL_INUM:  obj->data.inum = val->data.inum; break;
                case VAL_FNUM:  obj->data.inum = (int64_t)val->data.unum; break;
                case VAL_ADDRESS:  obj->data.inum = (int64_t)val->data.unum; break;
                // errors
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_ERROR:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_FNUM:
            switch(obj->type) {
                case VAL_UNUM:  obj->data.fnum = (double)val->data.unum; break;
                case VAL_INUM:  obj->data.fnum = (double)val->data.inum; break;
                case VAL_FNUM:  obj->data.fnum = val->data.fnum; break;
                case VAL_ADDRESS:  obj->data.fnum = (double)val->data.unum; break;
                // errors
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_ERROR:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_BOOL:
            switch(obj->type) {
                case VAL_BOOL:      obj->data.boolean = val->data.boolean; break;
                case VAL_STRING:    obj->data.boolean = val->isAssigned? true: false; break;
                case VAL_UNUM:      obj->data.boolean = val->isAssigned? true: false; break;
                case VAL_INUM:      obj->data.boolean = val->isAssigned? true: false; break;
                case VAL_FNUM:      obj->data.boolean = val->isAssigned? true: false; break;
                case VAL_ADDRESS:   obj->data.boolean = val->isAssigned? true: false; break;
                case VAL_ERROR:     obj->data.boolean = val->isAssigned? true: false; break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        case VAL_ADDRESS:
            switch(obj->type) {
                case VAL_UNUM:  obj->data.unum = val->data.unum; break;
                case VAL_INUM:  obj->data.unum = (uint64_t)val->data.inum; break;
                case VAL_ADDRESS: obj->data.unum = val->data.unum; break;
                // errors
                case VAL_FNUM:
                case VAL_STRING:
                case VAL_ERROR:
                case VAL_BOOL:
                    runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                    break;
                default:
                    runtimeError("attempt to assign an unknown object type: %d", obj->type);
            }
            break;
        default:
            runtimeError("attempt to assign an unknown object type: %d", obj->type);
        }
    }
}

const char* valToStr(ValueType type)
{
    return (type == VAL_ERROR)? "ERROR":
            (type == VAL_UNUM)? "UNUM":
            (type == VAL_INUM)? "INUM":
            (type == VAL_FNUM)? "FNUM":
            (type == VAL_BOOL)? "BOOL":
            (type == VAL_STRING)? "STRING":
            (type == VAL_ADDRESS)? "ADDRESS": "UNKNOWN";
}

