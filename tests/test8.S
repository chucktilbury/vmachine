/*
 * Test the jmpif instruction.
 */

start
    push 2
    push 2
    add
    push 4
    sub
    jmpif dest
    push 88
    print
    pop
    jmp end
dest
    push 10
    print
    pop
end
    exit
