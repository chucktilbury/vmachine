#ifndef _RUN_ARITH_H_
#define _RUN_ARITH_H_

int do_OP_ADD(VMachine* vm);
int do_OP_SUB(VMachine* vm);
int do_OP_MUL(VMachine* vm);
int do_OP_DIV(VMachine* vm);
int do_OP_MOD(VMachine* vm);
int do_OP_NEG(VMachine* vm);
int do_OP_CAST(VMachine* vm);

#endif