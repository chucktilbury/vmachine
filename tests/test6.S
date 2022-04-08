/*
 * See if comparisons work
 */

bool val1 = true
bool val2 = false
bool val3 = true

label
    push val1
    push val2
    eq
    print
    pop

    push val3
    push val1
    neq
    print
    pop

    push 123
    push 0
    leq
    print
    pop

    push 123
    push 0
    geq
    print
    pop

    push 123
    push 0
    less
    print
    pop

    push 123
    push 0
    gtr
    print
    pop

    exit