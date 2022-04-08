#ifndef _RUN_PUSH_H_
#define _RUN_PUSH_H_

int do_OP_PUSH(VMachine* vm);
int do_OP_PUSH8(VMachine* vm);
int do_OP_PUSH16(VMachine* vm);
int do_OP_PUSH32(VMachine* vm);
int do_OP_POP(VMachine* vm);

#endif