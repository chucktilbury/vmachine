
#include <math.h>
#include <stdarg.h>

#include "common.h"
#include "vm_support.h"
#include "scanner.h"

void castToType(Value* val, ValueType type)
{
    switch(type) {
        case VAL_UNUM: {
                uint64_t v;
                switch(val->type) {
                    case VAL_UNUM: break;
                    case VAL_INUM: v = (uint64_t)val->data.inum; break;
                    case VAL_FNUM: v = (uint64_t)((int64_t)val->data.fnum); break;
                    default:
                        runtimeError("cannot cast a %s to a %s", valToStr(val->type), valToStr(type));
                        break;
                }
                val->type = VAL_UNUM;
                val->data.unum = v;
            }
            break;

        case VAL_INUM: {
                int64_t v;
                switch(val->type) {
                    case VAL_UNUM: v = (int64_t)val->data.unum; break;
                    case VAL_INUM: break;
                    case VAL_FNUM: v = (int64_t)val->data.fnum; break;
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
                    case VAL_UNUM: v = (double)((int64_t)val->data.unum); break;
                    case VAL_INUM: v = (double)val->data.fnum; break;
                    case VAL_FNUM: break;
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
}

void addVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum + right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum + (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum + (uint64_t)((int64_t)right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum + (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum + right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum + (int64_t)right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum + (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum + (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum + right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
}

void subVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum - right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum - (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum - (uint64_t)((int64_t)right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum - (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum - right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum - (int64_t)right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum - (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum - (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum - right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
}

void mulVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum * right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum * (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum * (uint64_t)((int64_t)right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum * (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum * right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum * (int64_t)right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum * (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum * (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum * right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
}

void divVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum / right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum / (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum / (uint64_t)((int64_t)right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum / (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum / right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum / (int64_t)right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum / (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum / (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum / right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
}

void modVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                runtimeError("divide by zero");
                return;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum % right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum % (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum % (uint64_t)((int64_t)right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum % (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum % right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum % (int64_t)right.data.fnum; break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = fmod(left.data.fnum, (double)right.data.unum); break;
                case VAL_INUM: dest->data.fnum = fmod(left.data.fnum, (double)right.data.inum); break;
                case VAL_FNUM: dest->data.fnum = fmod(left.data.fnum, right.data.fnum); break;
                default: runtimeError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            runtimeError("only numbers allowed in expressions");
            break;
    }
}

void negVal(Value* dest, Value val)
{
    dest->type = val.type;
    switch(val.type) {
        case VAL_UNUM: dest->data.unum = (uint64_t)(-(int64_t)val.data.unum); break;
        case VAL_INUM: dest->data.unum = -val.data.inum; break;
        case VAL_FNUM: dest->data.unum = -val.data.fnum; break;
        default: runtimeError("only numbers allowed in expressions"); break;
    }
}

