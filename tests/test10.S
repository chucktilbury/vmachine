/*
 * In this test, the function named "dest" has 2 paramaters.
 * They are used in the calculation and the result is returned
 * on the top of the stack and printed.
 */

start
    push 70     // push 70
    push 100    // push 100
    call dest   // call test function
    prints      // 6. print 619000
    push 1000
    div
    prints      // 7. print 619
    pop
    jmp end

dest
    push 5000
    push 17
    peek -2     // push 70 on the stack
    prints      // 1. print 70
    mul         // 70 * 17 = 1190
    prints      // 2. print 1190
    add         // 5000 + 1190 = 6190
    prints      // 3. print 6190
    peek -1     // push 100
    prints      // 4. print 100
    mul
    prints      // 5. print 619000
    ret         // return the top of the stack

end
    exit


