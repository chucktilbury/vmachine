#ifndef _RUN_JMPIF_H_
#define _RUN_JMPIF_H_

int do_OP_JMPIF(VMachine* vm);
int do_OP_JMPIF8(VMachine* vm);
int do_OP_JMPIF16(VMachine* vm);
int do_OP_JMPIF32(VMachine* vm);
int do_OP_JMPIFR(VMachine* vm);
int do_OP_JMPIFR8(VMachine* vm);
int do_OP_JMPIFR16(VMachine* vm);
int do_OP_JMPIFR32(VMachine* vm);

#endif
