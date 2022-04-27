/*
    Multi line comment
 */
startup
    call entry
    exit

int some_name = 22 + 3
int another_name = -123

_symbol_123
    push some_name
    pUSh another_name
    add
    prints
    push some_name
    div
    prints
    noop
    ret

entry
    call _symbol_123
    ret
