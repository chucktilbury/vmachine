#ifndef _RUN_OTHER_H_
#define _RUN_OTHER_H_

int do_OP_NOOP(VMachine* vm);
int do_OP_EXIT(VMachine* vm);
int do_OP_PRINT(VMachine* vm);
int do_OP_EXCEPT(VMachine* vm);
int do_OP_SAVE(VMachine* vm);
int do_OP_ERROR(VMachine* vm);

#endif