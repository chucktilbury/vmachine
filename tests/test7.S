/*
 * Check the JMP instructions.
 */

entry
    jmp dest
    push 10
    push 10
    mul
    print
    pop
dest
    push 5
    push 5
    add
    print
    pop
    exit