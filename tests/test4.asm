
/*
    Verify that addresses are being generated correctly.
 */

int some_name = 22 + 3
int another_name = -123

_symbol_123
    push some_name
    pUSh another_name
    add
    print
    push some_name
    div
    print
    noop

uint uint_name = 22 + 3
int int_name = 123

_symbol_321
    push uint_name
    pUSh int_name
    add
    print
    push some_name
    div
    print
    noop

const int some_int = 22 + 3
float float_name = 102

_symbol_
    push some_int
    pUSh float_name
    add
    print
    push some_name
    div
    print
    noop
    exit

