/*
 *  Execute a simple loop.
 *
 *  Pseudo code:
 *  void func() {
 *      print 10;
 *  }
 *
 *  main() {
 *      int value = 10;
 *      while(value > 0) {
 *          func();
 *          value -= 1;
 *      }
 *  }
 *
 */
entry
    call main
    exit

func
    push 188    // push something to print
    prints      // print it
    pop         // leave the stack clean
    ret         // return to main

main
    push 10     // push the counter
  top_of_loop
    call func   // call the stack-neutral func
    push 1      // value to subtract from counter
    sub         // this leaves one item on the stack
    prints      // print the result
    jmpif skippy    // if it was zero then take this jump
    jmp top_of_loop // else take this jump
  skippy        // exiting the loop
    // clean the stack. note that the sub just leaves one
    // item on the stack.
    pop
    ret         // return to entry