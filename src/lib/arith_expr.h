#ifndef _ARITH_EXPR_H_
#define _ARITH_EXPR_H_

#include "value.h"

void addVals(Value* dest, Value* left, Value* right);
void subVals(Value* dest, Value* left, Value* right);
void mulVals(Value* dest, Value* left, Value* right);
void divVals(Value* dest, Value* left, Value* right);
void modVals(Value* dest, Value* left, Value* right);
void negVal(Value* val);
void castToType(Value* val, ValType type);

#endif