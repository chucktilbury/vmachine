/*
 * Test if duplicate strings are properly stored and compared.
 */

string str1 = 'this is a string'
string str2 = 'this is a string'

entry
    call test1
    call test2
    exit

test2
    push str1
    push str2
    equ
    prints
    pop
    ret

string str3 = "this is x string"

// must have a label at the start of a instruction block
test1
    push str1
    push str3
    equ
    prints
    pop
    ret