#ifndef _EMIT_H_
#define _EMIT_H_

#include "var_store.h"

void emitJMP(Variable* val);
void emitJMPIF(Variable* val);
void emitCALL(Variable* val);
void emitJMPR(Variable* val);
void emitJMPIFR(Variable* val);
void emitCALLR(Variable* val);
void emitPUSH(Variable* val);
void emitPeek(Variable* val);
void emitLocal(Variable* val);

#endif