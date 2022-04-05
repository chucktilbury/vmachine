
#include "common.h"

/**
 * @brief Init an error value
 *
 * @param obj
 *
 */
static void init_error_obj(Value* obj)
{
    obj->isAssigned = true;
    obj->isConst = true;
    obj->isLiteral = true;
    obj->data.unum = 0;
}

/**
 * @brief Create a Val Store object
 *
 * @return ValList*
 */
ValList* createValList()
{
    ValList* obj = createGPL();

    Value* eobj = createVal(VAL_ERROR);
    init_error_obj(eobj);
    obj->list[0] = eobj;
    obj->len = 1;

    return obj;
}

/**
 * @brief Create a Val object
 *
 * @param type
 *
 * @return Value*
 */
Value* createVal(ValType type)
{
    Value* val = _alloc_ds(Value);
    val->type = type;
    val->isAssigned = false;
    val->isConst = false;
    val->isLiteral = false;
    val->data.unum = 0;

    return val;
}

/**
 * @brief Assign one value type to another, with error checking.
 *
 * @param obj
 * @param val
 *
 */
void assignVal(Value* obj, Value* val)
{
    switch(obj->type) {
        case VAL_OBJ: {
                switch(val->type) {
                    case VAL_OBJ:
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
                        runtimeError("7 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_ERROR:
                switch(val->type) {
                    case VAL_OBJ:
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
                        runtimeError("6 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_UNUM:
                switch(val->type) {
                    case VAL_UNUM:
                        obj->data.unum = val->data.unum;
                        break;
                    case VAL_INUM:
                        obj->data.unum = (uint64_t)val->data.inum;
                        break;
                    case VAL_FNUM:
                        obj->data.unum = (uint64_t)((int64_t)val->data.fnum);
                        break;
                    case VAL_ADDRESS:
                        obj->data.unum = val->data.unum;
                        break;
                    // errors
                    case VAL_BOOL:
                    case VAL_OBJ:
                    case VAL_ERROR:
                        runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                        break;
                    default:
                        runtimeError("5 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_INUM:
                switch(val->type) {
                    case VAL_UNUM:
                        obj->data.inum = (int64_t)val->data.unum;
                        break;
                    case VAL_INUM:
                        obj->data.inum = val->data.inum;
                        break;
                    case VAL_FNUM:
                        obj->data.inum = (int64_t)val->data.fnum;
                        break;
                    case VAL_ADDRESS:
                        obj->data.inum = (int64_t)val->data.unum;
                        break;
                    // errors
                    case VAL_BOOL:
                    case VAL_OBJ:
                    case VAL_ERROR:
                        runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                        break;
                    default:
                        runtimeError("4 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_FNUM:
                switch(val->type) {
                    case VAL_UNUM:
                        obj->data.fnum = (double)((int64_t)val->data.unum);
                        break;
                    case VAL_INUM:
                        obj->data.fnum = (double)val->data.inum;
                        break;
                    case VAL_FNUM:
                        obj->data.fnum = val->data.fnum;
                        break;
                    case VAL_ADDRESS:
                        obj->data.fnum = (double)((int64_t)val->data.unum);
                        break;
                    // errors
                    case VAL_BOOL:
                    case VAL_OBJ:
                    case VAL_ERROR:
                        runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                        break;
                    default:
                        runtimeError("3 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_BOOL:
                switch(val->type) {
                    case VAL_BOOL:
                        obj->data.boolean = val->data.boolean;
                        break;
                    case VAL_OBJ:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    case VAL_UNUM:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    case VAL_INUM:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    case VAL_FNUM:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    case VAL_ADDRESS:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    case VAL_ERROR:
                        obj->data.boolean = val->isAssigned ? true : false;
                        break;
                    default:
                        runtimeError("2 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            case VAL_ADDRESS:
                switch(val->type) {
                    case VAL_UNUM:
                        obj->data.unum = val->data.unum;
                        break;
                    case VAL_INUM:
                        obj->data.unum = (uint64_t)val->data.inum;
                        break;
                    case VAL_ADDRESS:
                        obj->data.unum = val->data.unum;
                        break;
                    // errors
                    case VAL_FNUM:
                    case VAL_OBJ:
                    case VAL_ERROR:
                    case VAL_BOOL:
                        runtimeError("cannot assign a %s to a %s", valToStr(val->type), valToStr(obj->type));
                        break;
                    default:
                        runtimeError("1 attempt to assign an unknown object type: %s", valToStr(obj->type));
                }
                break;
            default:
                runtimeError("xx attempt to assign an unknown object type: %s", valToStr(obj->type));
            }
    }
}

/**
 * @brief Convert a value type to a string for errors and debugging.
 *
 * @param type
 *
 * @return const char*
 */
const char* valToStr(ValType type)
{
    return (type == VAL_ERROR) ? "ERROR" :
           (type == VAL_UNUM) ? "UNUM" :
           (type == VAL_INUM) ? "INUM" :
           (type == VAL_FNUM) ? "FNUM" :
           (type == VAL_BOOL) ? "BOOL" :
           (type == VAL_OBJ) ? "OBJECT" :
           (type == VAL_ADDRESS) ? "ADDRESS" : "UNKNOWN";
}

void dumpVals(ValList* lst)
{
    resetGPL(lst);
    Value* val;
    while(NULL != (val = iterGPL(lst))) {
        printf("%3d. ", lst->idx);
        printVal(val);
    }
}

void printVal(Value* obj)
{
    printf("%-12s", valToStr(obj->type));
    int pad_len = 10;
    switch(obj->type) {
        case VAL_ERROR: {
                int slen = snprintf(NULL, 0, "ERROR");
                printf("ERROR%*s", pad_len - slen, " ");
            }
            break;
        case VAL_OBJ: {
                int slen = snprintf(NULL, 0, "%d", obj->data.obj);
                printf("%d%*s", obj->data.obj, pad_len - slen, " ");
            }
            break;
        case VAL_UNUM: {
                int slen = snprintf(NULL, 0, "0x%lX", obj->data.unum);
                printf("0x%lX%*s", obj->data.unum, pad_len - slen, " ");
            }
            break;
        case VAL_ADDRESS:
        case VAL_INUM: {
                int slen = snprintf(NULL, 0, "%ld", obj->data.inum);
                printf("%ld%*s", obj->data.inum, pad_len - slen, " ");
            }
            break;
        case VAL_FNUM: {
                int slen = snprintf(NULL, 0, "%0.1f", obj->data.fnum);
                printf("%0.1f%*s", obj->data.fnum, pad_len - slen, " ");
            }
            break;
        case VAL_BOOL: {
                int slen = snprintf(NULL, 0, "%s", obj->data.boolean ? "TRUE" : "FALSE");
                printf("%s%*s", obj->data.boolean ? "TRUE" : "FALSE", pad_len - slen, " ");
            }
            break;
        default:
            printf("object value not found");
            break;
    }

    printf("assigned: %s const: %s literal: %s\n",
           obj->isAssigned ? "true" : "false",
           obj->isConst ? "true" : "false",
           obj->isLiteral ? "true" : "false");
}

