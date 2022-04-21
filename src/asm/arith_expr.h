#ifndef _ARITH_EXPR_H_
#define _ARITH_EXPR_H_

#include "asm.h"
#include "var_store.h"

void addVals(Variable* dest, Variable* left, Variable* right);
void subVals(Variable* dest, Variable* left, Variable* right);
void mulVals(Variable* dest, Variable* left, Variable* right);
void divVals(Variable* dest, Variable* left, Variable* right);
void modVals(Variable* dest, Variable* left, Variable* right);
void negVal(Variable* val);
void castToType(Variable* val, uint16_t type);

#endif