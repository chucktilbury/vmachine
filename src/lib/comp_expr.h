#ifndef _COMP_EXPR_H_
#define _COMP_EXPR_H_

#include "value.h"

void notVal(Value* dest, Value* val);
void eqVal(Value* dest, Value* left, Value* right);
void neqVal(Value* dest, Value* left, Value* right);
void leqVal(Value* dest, Value* left, Value* right);
void geqVal(Value* dest, Value* left, Value* right);
void lessVal(Value* dest, Value* left, Value* right);
void gtrVal(Value* dest, Value* left, Value* right);

#endif