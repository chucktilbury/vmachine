/*
 *  Prove that a value can change at runtime and do more
 *  testing around functions.
 */

/*
 *  This implements the following code:
 *  int value3 = 0;
 *  main() {
 *      int value1 = 20;
 *      int value2 = 18;
 *      value3 = func1(value1, value2);
 *      print(value3);
 *  }
 *
 *  int func1(int v1, int v2) {
 *      return v1 * func2(v2);
 *  }
 *
 *  int func2(int v1) {
 *      return v1 * 5;
 *  }
 */

// these are global but they are used as locals. There is no notion of
// local at this level. Scope rules are enforced at the compiler level.
int value1 = 20
int value2 = 18

// global value
int value3 = 0

// program entry point
main
    push value1     // push the parameters of func1
    push value2     //
    // notice that this symbol is referenced before it is defined.
    call func1
    prints          // 5. func1 returns 1800
    print value3    // 6. value3 = 0
    save value3     // peek the stack into the var
    print value3    // 7. value3 now is 1800
    prints          // 8. stack is still 1800
    exit

func1
    peek -2     // push value1 on the stack as a parameter to func2
    call func2  // calling func 2
    prints      // 3. print 100
    peek -1     // get value2 on the stack for use
    mul
    prints      // 4. print 100 * 18 = 1800
    ret         // return 1800 on the stack

func2
    peek -1     // value1 was the parameter to func2
    prints      // 1. should print 20
    push 5
    mul
    prints      // 2. print 100
    ret         // return with 100 on the stack

/*
 *  Notice that the code has to carefully manipulate the stack. There is no
 *  mechanism by which locals are popped automatically. Functions are required
 *  to clean up any residue resulting from calculations. Local variables, which
 *  are kept on the stack must be popped as well. The compiler is responsible
 *  for tracking the "peek" index of local variables and function parameters.
 */