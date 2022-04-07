
/*
    Verify that addresses are being generated correctly.
 */

int some_name = (22 + 3) * 2
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
    PUSH int_name
    add
    print
    push some_name
    div
    print
    NoOp

const float some_int =  int_name
uint float_name = 102

_symbol_
    push some_int
    pUSh float_name
    add
    print
    push 27.12*3
    div
    print
    noop

_bacon
    cast float_name float
    push float_name
    push 12.6
    add
    print
    exit

