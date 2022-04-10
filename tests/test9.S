/*
 * Basic test for call/return
 */

start
    push 10     // push 10
    push 1      // push 1
    call dest   // stack is clear after return
    push 1      // push 1
    add         // add 1+1=2
    print
    div         // div 10 / 2
    print       // should print 5
    pop
    jmp end
dest
    push 2
    push 17
    mul
    print       // should print 34
    pop         // pop the result
    return
end
    exit