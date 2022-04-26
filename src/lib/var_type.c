
#include "var_type.h"

const char* varTypeToStr(ValType type)
{
    return (type == VAL_ERROR) ? "ERROR" :
           (type == VAL_NOTHING) ? "NOTHING" :
           (type == VAL_UNUM) ? "UNUM" :
           (type == VAL_INUM) ? "INUM" :
           (type == VAL_FNUM) ? "FNUM" :
           (type == VAL_BOOL) ? "BOOL" :
           (type == VAL_STRING) ? "STRING" :
           (type == VAL_STRUCT) ? "STRUCT" :
           (type == VAL_ADDRESS) ? "ADDRESS" : "UNKNOWN";
}
