#ifndef _JUMPS_H_
#define _JUMPS_H_

void emitJMP(VMachine* vm, Value* val);
void emitJMPIF(VMachine* vm, Value* val);
void emitCALL(VMachine* vm, Value* val);
void emitJMPR(VMachine* vm, Value* val);
void emitJMPIFR(VMachine* vm, Value* val);
void emitCALLR(VMachine* vm, Value* val);
void emitPUSH(VMachine* vm, Value* val);

#endif