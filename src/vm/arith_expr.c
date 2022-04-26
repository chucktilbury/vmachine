/*
 * Promotion rules:
 *
 * The result of a calculation has the greatest resolution of any operand in
 * the calculation. In other words, a float has the highest resolution, then
 * a signed int, then an unsigned int.
 *
 * An operation contaning a float and any other numeric type results in a
 * float.
 *
 * An operation containing an int and a uint result in an int.
 *
 * An operation containing only uints results in a uint.
 *
 * This code is miles of slog. It could be implemented as tables, but the
 * switch/case style is faster.
 */

#include <math.h>
#include <stdarg.h>

#include "common.h"
//#include "scanner.h"
#include "vm.h"


/**
 * @brief Change the type of the value to the type given by the parameter.
 *
 * @param val
 * @param type
 *
 */
StkVal castToType(StkVal val, ValType type)
{
    switch(type) {
        case VAL_UNUM: {
                switch(val.type) {
                    case VAL_UNUM:
                        break;
                    case VAL_INUM:
                        val.data.unum = (uint32_t)val.data.inum;
                        val.type = VAL_UNUM;
                        break;
                    case VAL_FNUM:
                        val.data.unum = (uint32_t)((int32_t)val.data.fnum);
                        val.type = VAL_UNUM;
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", varTypeToStr(val.type), varTypeToStr(type));
                        break;
                }
            }
            break;

        case VAL_INUM: {
                switch(val.type) {
                    case VAL_UNUM:
                        val.data.inum = (int32_t)val.data.unum;
                        val.type = VAL_INUM;
                        break;
                    case VAL_INUM:
                        break;
                    case VAL_FNUM:
                        val.data.inum = (int32_t)val.data.fnum;
                        val.type = VAL_INUM;
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", varTypeToStr(val.type), varTypeToStr(type));
                        break;
                }
            }
            break;

        case VAL_FNUM: {
                switch(val.type) {
                    case VAL_UNUM:
                        val.data.fnum = (double)((int32_t)val.data.unum);
                        val.type = VAL_FNUM;
                        break;
                    case VAL_INUM:
                        val.data.fnum = (double)val.data.fnum;
                        val.type = VAL_FNUM;
                        break;
                    case VAL_FNUM:
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", varTypeToStr(val.type), varTypeToStr(type));
                        break;
                }
            }
            break;

        default:
            runtimeError("cannot cast a %s to a %s", varTypeToStr(val.type), varTypeToStr(type));
            break;
    }
    //val.hash = hashValue(val);
    return val;
}

StkVal addVals(StkVal left, StkVal right)
{
    StkVal dest;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.unum = left.data.unum + right.data.unum;
                    dest.type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = (int32_t)left.data.unum + right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)((int32_t)left.data.unum) + right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.inum = left.data.inum + (int32_t)right.data.unum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = left.data.inum + right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)left.data.inum + right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.fnum = left.data.fnum + (double)((int32_t)right.data.unum);
                    dest.type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest.data.fnum = left.data.fnum + (double)right.data.inum;
                    dest.type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = left.data.fnum + right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
            break;
    }
    //dest.hash = hashValue(dest);
    return dest;
}

StkVal subVals(StkVal left, StkVal right)
{
    StkVal dest;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.unum = left.data.unum - right.data.unum;
                    dest.type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = (int32_t)left.data.unum - right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)((int32_t)left.data.unum) - right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.inum = left.data.inum - (int32_t)right.data.unum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = left.data.inum - right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)left.data.inum - right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.fnum = left.data.fnum - (double)((int32_t)right.data.unum);
                    dest.type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest.data.fnum = left.data.fnum - (double)right.data.inum;
                    dest.type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = left.data.fnum - right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
            break;
    }
    //dest.hash = hashValue(dest);
    return dest;
}

StkVal mulVals(StkVal left, StkVal right)
{
    StkVal dest;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.unum = left.data.unum * right.data.unum;
                    dest.type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = (int32_t)left.data.unum * right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)((int32_t)left.data.unum) * right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.inum = left.data.inum * (int32_t)right.data.unum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = left.data.inum * right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)left.data.inum * right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.fnum = left.data.fnum * (double)((int32_t)right.data.unum);
                    dest.type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest.data.fnum = left.data.fnum * (double)right.data.inum;
                    dest.type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = left.data.fnum * right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
            break;
    }
    //dest.hash = hashValue(dest);
    return dest;
}

StkVal divVals(StkVal left, StkVal right)
{
    StkVal dest;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions: %s", varTypeToStr(right.type));
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.unum = left.data.unum / right.data.unum;
                    dest.type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = (int32_t)left.data.unum / right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)((int32_t)left.data.unum) / right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions: %s", varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.inum = left.data.inum / (int32_t)right.data.unum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = left.data.inum / right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = (double)left.data.inum / right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions: %s", varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.fnum = left.data.fnum / (double)((int32_t)right.data.unum);
                    dest.type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest.data.fnum = left.data.fnum / (double)right.data.inum;
                    dest.type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = left.data.fnum / right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions: %s", varTypeToStr(right.type));
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions: %s", varTypeToStr(left.type));
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
            break;
    }
    //dest.hash = hashValue(dest);
    return dest;
}

StkVal modVals(StkVal left, StkVal right)
{
    StkVal dest;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                runtimeError("divide by zero");
                exit(1);
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.unum = left.data.unum % right.data.unum;
                    dest.type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = (int32_t)left.data.unum % right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = fmod((double)((int32_t)left.data.unum), right.data.fnum);
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.inum = left.data.inum % (int32_t)right.data.unum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest.data.inum = left.data.inum % right.data.inum;
                    dest.type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest.data.inum = left.data.inum % (int32_t)right.data.fnum;
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM:
                    dest.data.fnum = fmod(left.data.fnum, (double)((int32_t)right.data.unum));
                    dest.type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest.data.fnum = fmod(left.data.fnum, (double)right.data.inum);
                    dest.type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest.data.fnum = fmod(left.data.fnum, right.data.fnum);
                    dest.type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    dest.type = VAL_ERROR;
                    dest.data.unum = 0;
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            dest.type = VAL_ERROR;
            dest.data.unum = 0;
            break;
    }
    //dest.hash = hashValue(dest);
    return dest;
}

StkVal negVal(StkVal val)
{
    switch(val.type) {
        case VAL_UNUM:
            val.data.unum = -val.data.unum;
            break;
        case VAL_INUM:
            val.data.inum = -val.data.inum;
            break;
        case VAL_FNUM:
            val.data.fnum = -val.data.fnum;
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    //val.hash = hashValue(val);
    return val;
}

// peek at the top of the expression stack and return true if its boolean
// value can be considered false.
bool isFalse(StkVal val)
{
    switch(val.type) {
        case VAL_ERROR:
        case VAL_STRING:
        case VAL_STRUCT:
        case VAL_ADDRESS:
            return false;    // these are always true for this (so return false)
        case VAL_UNUM:
            return (val.data.unum == 0)? true: false;
        case VAL_INUM:
            return (val.data.inum == 0)? true: false;
        case VAL_FNUM:
            return (val.data.fnum == 0.0)? true: false;
        case VAL_BOOL:
            return val.data.boolean;
        default:
            return true;
    }

}