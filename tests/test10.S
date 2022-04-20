/*
 * In this test, the function named "dest" has 2 paramaters.
 * They are used in the calculation and the result is returned
 * on the top of the stack and printed.
 */

start
    push 70     // push 70
    push 100    // push 1
    push 2      // 2 call parameters
    call dest   // call test function
    prints       // 4. print 35
    pop
    jmp end
dest
    push 5000
    push 17
    peek 2      // push 70 on the stack
    mul         // 70 * 17 = 1190
    prints       // 1. print 1190
    add         // 5000 + 1190 = 6190
    prints       // 2. print 4
    peek 1      // push 100
    mul
    prints       // 3. print 619000
    ret         // return the top of the stack
end
    exit


