
#include "common.h"
#include "val_stack.h"
#include "memory.h"
//#include "var_store.h"

typedef struct _val_stack_ {
    StkVal* list;
    size_t cap;
    size_t len;
} ValStack;

static ValStack stack;

void createValStack()
{
    stack.cap = 0x01 << 8;
    stack.len = 0;
    stack.list = _alloc_ds_array(StkVal, stack.cap);
}

void destroyValStack()
{
    if(stack.list != NULL) {
        _free(stack.list);
    }
}

int getValStackLen()
{
    return stack.len;
}

StkVal initVal(uint16_t type, void* val)
{
    StkVal value;

    switch(type) {
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ERROR:
            value.data.store_idx = *(size_t*)val;
            break;
        case VAL_NOTHING:
            value.data.store_idx = 0;
            break;
        case VAL_UNUM:
            value.data.unum = *(uint32_t*)val;
            break;
        case VAL_INUM:
            //printf("inum = %d\n", *(int32_t*)val);
            value.data.inum = *(int32_t*)val;
            break;
        case VAL_FNUM:
            value.data.fnum = *(float*)val;
            break;
        case VAL_BOOL:
            value.data.inum = *(uint8_t*)val;
            break;
        case VAL_ADDRESS:
            value.data.inum = *(uint32_t*)val;
            break;
        default:
            fatalError("invalid value type in initVal(): 0x%02X\n", type);
    }
    value.type = type;

    return value;
}

void pushVal(StkVal val)
{
    if((stack.len + 1) > stack.cap) {
        stack.cap <<= 1;
        stack.list = _realloc_ds_array(stack.list, StkVal, stack.cap);
    }

    stack.list[stack.len] = val;
    stack.len++;
    //printf("TOS after push: %lu\t", stack.len);
}

StkVal popVal()
{
    // unsigned compare...
    if(stack.len - 1 > stack.len) {
        fatalError("pop value stack under run");
    }

    stack.len--;
    //printf("TOS after pop: %lu\t", stack.len);
    return stack.list[stack.len];
}

StkVal peekVal(size_t index)
{
    // note that offset could be negative...
    //size_t idx = (stack.len - 1) + offset;
    if(index > stack.len) {
        fatalError("peek value stack under run (%lu)", index);
    }

    //printf("offset: %d, stack: %d\n", offset, stack.list[offset].data.inum);
    return stack.list[index];
}

StkVal peekStk()
{
    if(stack.len - 1 > stack.len) {
        fatalError("value stack under run");
    }

    return stack.list[stack.len - 1];
}

//void printVal(StkVal val)
void printVal(uint8_t type, void* val)
{
    trace(1, "%s\t", varTypeToStr(type));
    switch(type) {
        case VAL_ERROR:
            trace(1, "ERROR");
            break;
        case VAL_STRING:
        case VAL_STRUCT:
            trace(1, "%ul", *(size_t*)val);
            break;
        case VAL_UNUM:
            trace(1, "0x%X", *((uint32_t*)val));
            break;
        case VAL_ADDRESS:
            trace(1, "%u", *((uint32_t*)val));
            break;
        case VAL_INUM:
            trace(1, "%d", *((int32_t*)val));
            break;
        case VAL_FNUM:
            trace(1, "%0.1f", *((float*)val));
            break;
        case VAL_BOOL:
            trace(1, "%s", *((bool*)val) ? "TRUE" : "FALSE");
            break;
        default:
            trace(1, "object value not found");
            break;
    }
}

/**
 * @brief Assign a value to a local var that has been allocated upon the
 * stack.
 *
 * @param index
 * @param val
 *
 */
void assignVal(int index, StkVal val)
{
    //printf("INDEX: %d\n", index);
    switch(stack.list[index].type) {
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ERROR:
            switch(val.type) {
                case VAL_ERROR:
                case VAL_STRING:
                case VAL_STRUCT:
                    stack.list[index].data.store_idx = val.data.store_idx;
                    break;
                case VAL_NOTHING:
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                    genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;

        case VAL_NOTHING:
            genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
            break;

        case VAL_UNUM:
            switch(val.type) {
                case VAL_ERROR:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_NOTHING:
                    genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
                    break;
                case VAL_UNUM:
                    stack.list[index].data.unum = val.data.unum;
                    break;
                case VAL_INUM:
                    stack.list[index].data.unum = (uint32_t)val.data.inum;
                    break;
                case VAL_FNUM:
                    stack.list[index].data.unum = (uint32_t)((int32_t)val.data.fnum);
                    break;
                case VAL_BOOL:
                    stack.list[index].data.unum = val.data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    stack.list[index].data.unum = val.data.addr;
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;

        case VAL_INUM:
            switch(val.type) {
                case VAL_ERROR:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_NOTHING:
                    genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
                    break;
                case VAL_UNUM:
                    stack.list[index].data.inum = (int32_t)val.data.unum;
                    break;
                case VAL_INUM:
                    stack.list[index].data.inum = val.data.inum;
                    break;
                case VAL_FNUM:
                    stack.list[index].data.inum = (int32_t)val.data.fnum;
                    break;
                case VAL_BOOL:
                    stack.list[index].data.inum = val.data.boolean ? 1 : 0;
                    break;
                case VAL_ADDRESS:
                    stack.list[index].data.inum = (int32_t)val.data.addr;
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;

        case VAL_FNUM:
            switch(val.type) {
                case VAL_ERROR:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_NOTHING:
                case VAL_ADDRESS:
                    genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
                    break;
                case VAL_UNUM:
                    stack.list[index].data.fnum = (float)((int32_t)val.data.unum);
                    break;
                case VAL_INUM:
                    stack.list[index].data.fnum = (float)val.data.inum;
                    break;
                case VAL_FNUM:
                    stack.list[index].data.fnum = val.data.fnum;
                    break;
                case VAL_BOOL:
                    stack.list[index].data.fnum = val.data.boolean ? 1 : 0;
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;

        case VAL_BOOL:
            switch(val.type) {
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_NOTHING:
                case VAL_ADDRESS:
                    stack.list[index].data.boolean = true;
                    break;
                case VAL_UNUM:
                    stack.list[index].data.boolean = val.data.unum ? true : false;
                    break;
                case VAL_INUM:
                    stack.list[index].data.boolean = val.data.inum ? true : false;
                    break;
                case VAL_FNUM:
                    stack.list[index].data.boolean = val.data.fnum != 0.0 ? true : false;
                    break;
                case VAL_BOOL:
                    stack.list[index].data.boolean = val.data.boolean;
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;
        case VAL_ADDRESS:
            switch(val.type) {
                case VAL_ERROR:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_NOTHING:
                case VAL_FNUM:
                case VAL_BOOL:
                    genericError("cannot assign a %s to a %s\n", varTypeToStr(val.type), varTypeToStr(stack.list[index].type));
                    break;
                case VAL_UNUM:
                    stack.list[index].data.addr = val.data.unum;
                    break;
                case VAL_INUM:
                    stack.list[index].data.addr = (uint32_t)val.data.inum;
                    break;
                case VAL_ADDRESS:
                    stack.list[index].data.addr = val.data.addr;
                    break;
                default:
                    fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
            }
            break;

        default:
            fatalError("cannot assign unknown value type: %u\n", stack.list[index].type);
    }
}

