/*
    Test aritmetic on signed integers
 */
startup
    call entry
    exit

int some_name = 22 + 3
int another_name = 123

_test1
    push some_name
    pUSh another_name
    add
    prints
    push some_name
    div
    prints
    noop
    return

entry
    call _test1
    ret
