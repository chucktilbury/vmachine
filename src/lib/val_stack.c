
#include "common.h"
#include "val_stack.h"
#include "memory.h"
//#include "var_store.h"

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
        case VAL_OBJ:
        case VAL_ERROR:
            value.data.obj = val;
            break;
        case VAL_NOTHING:
            value.data.obj = NULL;
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
            fprintf(stderr, "Fatal Error: invalid value type in initVal(): 0x%02X\n", type);
            exit(1);
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
        fprintf(stderr, "pop value stack under run\n");
        exit(1);
    }

    stack.len--;
    //printf("TOS after pop: %lu\t", stack.len);
    return stack.list[stack.len];
}

StkVal peekVal()
{
    if(stack.len - 1 > stack.len) {
        fprintf(stderr, "peek value stack under run\n");
        exit(1);
    }

    return stack.list[stack.len - 1];
}

//void printVal(StkVal val)
void printVal(uint8_t type, void* val)
{
    printf("%-12s", varTypeToStr(type));
    switch(type) {
        case VAL_ERROR:
            printf("ERROR");
            break;
        case VAL_OBJ:
            printf("%p", val);
            break;
        case VAL_UNUM:
            printf("0x%X", *((uint32_t*)val));
            break;
        case VAL_ADDRESS:
            printf("%u", *((uint32_t*)val));
            break;
        case VAL_INUM:
            printf("%d", *((int32_t*)val));
            break;
        case VAL_FNUM:
            printf("%0.1f", *((float*)val));
            break;
        case VAL_BOOL:
            printf("%s", *((bool*)val) ? "TRUE" : "FALSE");
            break;
        default:
            printf("object value not found");
            break;
    }
}

