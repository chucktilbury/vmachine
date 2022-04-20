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
    push value1 //
    push value2
    push 2
    call func1
    print value3
    save value3
    print value3
    exit

func1
    peek 2
    push 1
    call func2
    prints
    peek 1
    mul
    ret

func2
    peek 1
    prints
    push 5
    prints
    mul
    ret