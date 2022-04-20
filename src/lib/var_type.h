#ifndef _OBJECT_H_
#define _OBJECT_H_

typedef enum {
    VAL_ERROR,
    VAL_NOTHING,
    VAL_UNUM,
    VAL_INUM,
    VAL_FNUM,
    VAL_BOOL,
    VAL_OBJ,
    VAL_ADDRESS,
} ValType;

#endif