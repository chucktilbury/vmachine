#ifndef _PARS_SUPPORT_H_
#define _PARS_SUPPORT_H_

#include "value.h"

void addVals(Value* dest, Value left, Value right);
void subVals(Value* dest, Value left, Value right);
void mulVals(Value* dest, Value left, Value right);
void divVals(Value* dest, Value left, Value right);
void modVals(Value* dest, Value left, Value right);
void negVal(Value* dest, Value val);
void syntaxError(const char* fmt, ...);
void castToType(Value* val, ValueType type);

#endif