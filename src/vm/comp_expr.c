
#include "common.h"
#include "vm.h"



/******************************************************************************
 * Comparison expressions
 *
 *   OP_NOT,     // unary not conditional
 *   OP_EQ,      // equal conditional
 *   OP_NEQ,     // not equal conditional
 *   OP_LEQ,     // less-or-equal conditional
 *   OP_GEQ,     // greater-or-equal conditional
 *   OP_LESS,    // less than conditional
 *   OP_GTR,     // greater than conditional
 */

StkVal notVal(StkVal val)
{
    StkVal dest;
    switch(val.type) {
        case VAL_UNUM:
            dest.data.boolean = (val.data.unum == 0)? true: false;
            break;
        case VAL_INUM:
            dest.data.boolean = (val.data.inum == 0)? true: false;
            break;
        case VAL_FNUM:
            dest.data.boolean = (val.data.fnum == 0.0)? true: false;
            break;
        case VAL_BOOL:
            dest.data.boolean = (val.data.boolean == false)? true: false;
            break;
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ERROR:
        case VAL_ADDRESS:
            dest.data.boolean = true;
            break;
        default:
            runtimeError("unknown value type (%d) in comparison", val.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    dest.type = VAL_BOOL;
    return dest;
}

StkVal eqVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum == right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum == right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) == right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.unum == 0) == right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    // type comparison
                    dest.data.boolean = false;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum == (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum == right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum == right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.inum == 0) == right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    // type comparison
                    dest.data.boolean = false;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum == (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum == (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum == right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.fnum == 0.0) == right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    // type comparison
                    dest.data.boolean = false;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.boolean == (right.data.unum == 0));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.boolean == (right.data.inum == 0));
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.boolean == (right.data.fnum == 0.0));
                    break;
                case VAL_BOOL:
                    dest.data.boolean = (left.data.boolean == right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    // type comparison
                    dest.data.boolean = false;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_STRING:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_ADDRESS:
                case VAL_STRUCT:
                case VAL_ERROR:
                    dest.data.boolean = false;
                    break;
                case VAL_STRING:
                    // duplicate string are never stored.
                    dest.data.boolean = left.data.store_idx == right.data.store_idx? true: false;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_STRUCT:
        // TODO: add right clauses
            dest.data.boolean = false;
            break;
        case VAL_ADDRESS:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                    dest.data.boolean = false;
                    break;
                case VAL_ADDRESS:
                    // type comparison
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ADDRESS:
                    dest.data.boolean = false;
                    break;
                case VAL_ERROR:
                    // type comparison
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '==' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type (%d) in '==' comparison", left.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

StkVal neqVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum != right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum != right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) != right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.unum == 0) != right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum != (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum != right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum != right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.inum == 0) != right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum != (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum != (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum != right.data.fnum);
                    break;
                case VAL_BOOL:
                    dest.data.boolean = ((left.data.fnum == 0.0) != right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.boolean != (right.data.unum == 0));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.boolean != (right.data.inum == 0));
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.boolean != (right.data.fnum == 0.0));
                    break;
                case VAL_BOOL:
                    dest.data.boolean = (left.data.boolean != right.data.boolean);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_STRING:
        case VAL_STRUCT:
            // TODO: Implement right clauses
            dest.data.boolean = false;
            break;
        case VAL_ADDRESS:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    dest.data.boolean = true;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '!=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type %s in '!=' comparison", varTypeToStr(left.type));
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

StkVal leqVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum <= right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum <= right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) <= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum <= (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum <= right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum <= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum <= (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum <= (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum <= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ADDRESS:
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type (%d) in '<=' comparison", left.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

StkVal geqVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum >= right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum >= right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) >= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum >= (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum >= right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum >= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum >= (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum >= (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum >= right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ADDRESS:
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type (%d) in '>=' comparison", left.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

StkVal lessVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum < right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum < right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) < right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum < (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum < right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum < right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum < (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum < (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum < right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ADDRESS:
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type (%d) in '<' comparison", left.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

StkVal gtrVal(StkVal left, StkVal right)
{
    StkVal dest;
    dest.type = VAL_BOOL;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.unum > right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = ((int32_t)left.data.unum > right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)((int32_t)left.data.unum) > right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.inum > (int32_t)right.data.unum);
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.inum > right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = ((float)left.data.inum > right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.boolean = (left.data.fnum > (float)((int32_t)right.data.unum));
                    break;
                case VAL_INUM:
                    dest.data.boolean = (left.data.fnum > (float)right.data.inum);
                    break;
                case VAL_FNUM:
                    dest.data.boolean = (left.data.fnum > right.data.fnum);
                    break;
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_BOOL:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '>' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        case VAL_BOOL:
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ADDRESS:
        case VAL_ERROR:
            switch(right.type) {
                case VAL_UNUM:
                case VAL_INUM:
                case VAL_FNUM:
                case VAL_BOOL:
                case VAL_STRING:
                case VAL_STRUCT:
                case VAL_ERROR:
                case VAL_ADDRESS:
                    runtimeError("magnitude comparison between %s and %s address is meaningless",
                                    varTypeToStr(left.type), varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
                default:
                    runtimeError("unknown right value type (%d) in '<=' comparison", right.type);
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
            }
            break;
        default:
            runtimeError("unknown left value type (%d) in '>' comparison", left.type);
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
    }

    return dest;
}

