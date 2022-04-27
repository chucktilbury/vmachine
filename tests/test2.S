/*
    Test aritmetic on unsigned integers
 */
startup
    call entry
    exit

uint some_name = 22 + 3
uint another_name = 2345

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
    return
