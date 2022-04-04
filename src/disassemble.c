
// stand-alone disassembler.
#include "common.h"
#include <stdarg.h>

#include "vm_support.h"

void disassemble(VMachine* vm)
{
    bool finished = false;
    int inst = 0;

    while(!finished) {
        printf("%04d: ", IP(vm));
        inst = READ8(vm);
        //printf("instruction: %s\n", opToStr(instruction));
        switch(inst) {
            case OP_EXIT:
                printf("%s\n", opToStr(inst));
                finished = true;
                break;

            // no operand
            case OP_ERROR:
            case OP_NOOP:
            case OP_RETURN:
            case OP_POP:
            // unary operators
            case OP_NOT:
            case OP_NEG:
            // binary operators
            case OP_EQ:
            case OP_NEQ:
            case OP_LEQ:
            case OP_GEQ:
            case OP_LESS:
            case OP_GTR:
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
            case OP_PRINT: {
                    printf("%s\n", opToStr(inst));
                }
                break;

            // 16 bit operand
            case OP_CALL:
            case OP_CALLX:
            case OP_SAVE:
            case OP_PUSH: {
                    uint16_t oper = READ16(vm);
                    printf("%s\t%4d\t", opToStr(inst), oper);
                    printVal(getVal(vm->val_store, oper));
                }
                break;

            // 16 bit signed operand
            case OP_JMP:
            case OP_JMPIF: {
                    int oper = (int)READ16(vm);
                    printf("%s\t%d\t", opToStr(inst), oper);
                    printVal(getVal(vm->val_store, oper));
                }
                break;

            case OP_EXCEPT:
                printf("%s\t0x%08X", opToStr(inst), READ16(vm));
                break;

            case OP_CAST: {
                    int oper = (int)READ16(vm);
                    printf("%s\t%d\t", opToStr(inst), oper);
                    printVal(getVal(vm->val_store, oper));
                    oper = (int)READ8(vm);
                    printf("          cast to %s\n", valToStr(oper));
                }
                break;

            default:
                printf("invalid instruction: 0x%02X at 0x%0d\n", inst, IP(vm));
                exit(1);

        }
    }
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "use: %s filename\n", argv[0]);
        exit(1);
    }

    VMachine* vm = loadVM(argv[1]);
    disassemble(vm);

    dumpVals(vm->val_store);

    return 0;
}