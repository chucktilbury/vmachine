/*
 * get the save instruciton working
 */

int some_val    // un initialized var

label
    noop
    push 123
    push 2
    mul             // top of stack = 246
    save some_val   // save it to the val
    pop             // pop the stack
    push some_val
    push 2
    div
    print           // should print 123
    noop
    exit