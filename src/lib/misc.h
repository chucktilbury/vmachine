#ifndef _MISC_H_
#define _MISC_H_

#include "var_store.h"
#include "val_stack.h"

void createVMachine();
void destroyVMachine();

Variable* convertValToVar(StkVal val);
StkVal convertVarToVal(Variable* var);
void assignValToVar(Variable* var, StkVal val);

#endif