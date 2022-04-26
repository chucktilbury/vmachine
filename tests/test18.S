/*
 * Test if duplicate strings are properly stored and compared.
 */

string str1 = 'this is a string'
string str2 = 'this is a string'

entry
    push str1
    push str2
    equ
    prints
    pop

string str3 = "this is x string"

// must have a label at the start of a instruction block
label
    push str1
    push str3
    equ
    prints
    pop
    exit
