#ifndef _RUN_JMP_H_
#define _RUN_JMP_H_

int do_OP_JMP(VMachine* vm);
int do_OP_JMP8(VMachine* vm);
int do_OP_JMP16(VMachine* vm);
int do_OP_JMP32(VMachine* vm);
int do_OP_JMPR(VMachine* vm);
int do_OP_JMPR8(VMachine* vm);
int do_OP_JMPR16(VMachine* vm);
int do_OP_JMPR32(VMachine* vm);

#endif