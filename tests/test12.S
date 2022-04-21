/*
 *  Prove that a value can change at runtime and do more
 *  testing around functions.
 */

/*
 *  This implements the following code:
 *  main() {
 *      int value1 = 20;
 *      int value2 = 18;
 *      value1 = func1(value1, value2);
 *  }
 *
 *  int func1(int v1, int v2) {
 *      return v1 * func2(v2);
 *  }
 *
 */

// these are global but they are used as locals. There is no notion of
// local at this level. Scope rules are enforced at the compiler level.
int value1 = 20
int value2 = 18

// global value
int value3 = 0

// program entry point
main
    // value1 is a local and also a parameter of func1
    push value1     // push the parameters of func1
    push value2     //

    call func1
    prints      // 2. print 360 from call to func1
    local 1     // local 2 is the address of value1
    peek 1      // get value1 on the stack
    prints      // 3. should be 360
    exit

func1
    peek -2     // push value1 on the stack as a parameter to func2
    peek -1     // get value2 on the stack for use
    mul
    prints      // 1. print 20 * 18 = 360
    ret         // return 360 on the stack

/*
 *  Notice that the code has to carefully manipulate the stack. There is no
 *  mechanism by which locals are popped automatically. Functions are required
 *  to clean up any residue resulting from calculations. Local variables, which
 *  are kept on the stack must be popped as well. The compiler is responsible
 *  for tracking the "peek" index of local variables and function parameters.
 */