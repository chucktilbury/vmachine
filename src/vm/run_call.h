#ifndef _RUN_CALL_H_
#define _RUN_CALL_H_

int do_OP_CALLX(VMachine* vm);
int do_OP_CALL(VMachine* vm);
int do_OP_CALL8(VMachine* vm);
int do_OP_CALL16(VMachine* vm);
int do_OP_CALL32(VMachine* vm);
int do_OP_CALLR(VMachine* vm);
int do_OP_CALLR8(VMachine* vm);
int do_OP_CALLR16(VMachine* vm);
int do_OP_CALLR32(VMachine* vm);
int do_OP_RETURN(VMachine* vm);

#endif