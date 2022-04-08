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
#include "scanner.h"

/**
 * @brief Change the type of the value to the type given by the parameter.
 *
 * @param val
 * @param type
 *
 */
void castToType(Value* val, ValType type)
{
    switch(type) {
        case VAL_UNUM: {
                uint32_t v;
                switch(val->type) {
                    case VAL_UNUM:
                        break;
                    case VAL_INUM:
                        v = (uint32_t)val->data.inum;
                        break;
                    case VAL_FNUM:
                        v = (uint32_t)((int32_t)val->data.fnum);
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", valToStr(val->type), valToStr(type));
                        break;
                }
                val->type = VAL_UNUM;
                val->data.unum = v;
            }
            break;

        case VAL_INUM: {
                int32_t v;
                switch(val->type) {
                    case VAL_UNUM:
                        v = (int32_t)val->data.unum;
                        break;
                    case VAL_INUM:
                        break;
                    case VAL_FNUM:
                        v = (int32_t)val->data.fnum;
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", valToStr(val->type), valToStr(type));
                        break;
                }
                val->type = VAL_INUM;
                val->data.inum = v;
            }
            break;

        case VAL_FNUM: {
                double v;
                switch(val->type) {
                    case VAL_UNUM:
                        v = (double)((int32_t)val->data.unum);
                        break;
                    case VAL_INUM:
                        v = (double)val->data.fnum;
                        break;
                    case VAL_FNUM:
                        break;
                    default:
                        runtimeError("cannot cast a %s to a %s", valToStr(val->type), valToStr(type));
                        break;
                }
                val->type = VAL_FNUM;
                val->data.fnum = v;
            }
            break;

        default:
            runtimeError("cannot cast a %s to a %s", valToStr(val->type), valToStr(type));
            break;
    }
    val->hash = hashValue(val);
}

void addVals(Value* dest, Value* left, Value* right)
{
    switch(left->type) {
        case VAL_UNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.unum = left->data.unum + right->data.unum;
                    dest->type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = (int32_t)left->data.unum + right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)((int32_t)left->data.unum) + right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_INUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.inum = left->data.inum + (int32_t)right->data.unum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = left->data.inum + right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)left->data.inum + right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.fnum = left->data.fnum + (double)((int32_t)right->data.unum);
                    dest->type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest->data.fnum = left->data.fnum + (double)right->data.inum;
                    dest->type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = left->data.fnum + right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    dest->hash = hashValue(dest);

}

void subVals(Value* dest, Value* left, Value* right)
{
    switch(left->type) {
        case VAL_UNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.unum = left->data.unum - right->data.unum;
                    dest->type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = (int32_t)left->data.unum - right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)((int32_t)left->data.unum) - right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_INUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.inum = left->data.inum - (int32_t)right->data.unum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = left->data.inum - right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)left->data.inum - right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.fnum = left->data.fnum - (double)((int32_t)right->data.unum);
                    dest->type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest->data.fnum = left->data.fnum - (double)right->data.inum;
                    dest->type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = left->data.fnum - right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    dest->hash = hashValue(dest);
}

void mulVals(Value* dest, Value* left, Value* right)
{
    switch(left->type) {
        case VAL_UNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.unum = left->data.unum * right->data.unum;
                    dest->type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = (int32_t)left->data.unum * right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)((int32_t)left->data.unum) * right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_INUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.inum = left->data.inum * (int32_t)right->data.unum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = left->data.inum * right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)left->data.inum * right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.fnum = left->data.fnum * (double)((int32_t)right->data.unum);
                    dest->type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest->data.fnum = left->data.fnum * (double)right->data.inum;
                    dest->type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = left->data.fnum * right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    dest->hash = hashValue(dest);
}

void divVals(Value* dest, Value* left, Value* right)
{
    switch(right->type) {
        case VAL_UNUM:
            if(right->data.unum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right->data.inum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right->data.fnum == 0.0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }

    switch(left->type) {
        case VAL_UNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.unum = left->data.unum / right->data.unum;
                    dest->type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = (int32_t)left->data.unum / right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)((int32_t)left->data.unum) / right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_INUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.inum = left->data.inum / (int32_t)right->data.unum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = left->data.inum / right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = (double)left->data.inum / right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.fnum = left->data.fnum / (double)((int32_t)right->data.unum);
                    dest->type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest->data.fnum = left->data.fnum / (double)right->data.inum;
                    dest->type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = left->data.fnum / right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    dest->hash = hashValue(dest);
}

void modVals(Value* dest, Value* left, Value* right)
{
    switch(right->type) {
        case VAL_UNUM:
            if(right->data.unum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right->data.inum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right->data.fnum == 0.0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }

    switch(left->type) {
        case VAL_UNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.unum = left->data.unum % right->data.unum;
                    dest->type = VAL_UNUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = (int32_t)left->data.unum % right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = fmod((double)((int32_t)left->data.unum), right->data.fnum);
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_INUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.inum = left->data.inum % (int32_t)right->data.unum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_INUM:
                    dest->data.inum = left->data.inum % right->data.inum;
                    dest->type = VAL_INUM;
                    break;
                case VAL_FNUM:
                    dest->data.inum = left->data.inum % (int32_t)right->data.fnum;
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        case VAL_FNUM:
            switch(right->type) {
                case VAL_UNUM:
                    dest->data.fnum = fmod(left->data.fnum, (double)((int32_t)right->data.unum));
                    dest->type = VAL_FNUM;
                    break;
                case VAL_INUM:
                    dest->data.fnum = fmod(left->data.fnum, (double)right->data.inum);
                    dest->type = VAL_FNUM;
                    break;
                case VAL_FNUM:
                    dest->data.fnum = fmod(left->data.fnum, right->data.fnum);
                    dest->type = VAL_FNUM;
                    break;
                default:
                    runtimeError("only numbers allowed in expressions");
                    break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    dest->hash = hashValue(dest);
}

void negVal(Value* val)
{
    switch(val->type) {
        case VAL_UNUM:
            val->data.unum = -val->data.unum;
            break;
        case VAL_INUM:
            val->data.inum = -val->data.inum;
            break;
        case VAL_FNUM:
            val->data.fnum = -val->data.fnum;
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
    val->hash = hashValue(val);
}

