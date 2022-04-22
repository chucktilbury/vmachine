#ifndef MISC_H
#define MISC_H

#include "var_store.h"
#include "val_stack.h"

void createVMachine();
void destroyVMachine();

Variable* convertValToVar(StkVal val);
StkVal convertVarToVal(Variable* var);
void assignValToVar(Variable* var, StkVal val);

void paddedPrintBefore(int len, const char* fmt, ...);
void paddedPrintAfter(int len, const char* fmt, ...);

#endif