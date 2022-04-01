
// stand-alone disassembler.
#include "common.h"
#include <stdarg.h>

#include "vm_support.h"

void disassemble(VirtualMachine* vm)
{
    bool finished = false;
    int inst = 0;

    while(!finished) {
        inst = READ8(vm);
        //printf("instruction: %s\n", opToStr(instruction));
        switch(inst) {
            case OP_EXIT:
                printf("%08d: %s\n", IP(vm), opToStr(inst));
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
                printf("%08d: %s\n", IP(vm), opToStr(inst));
                break;
            }

            // 16 bit operand
            case OP_CALL:
            case OP_CALLX:
            case OP_PUSH:
            case OP_FREE: {
                uint16_t oper = READ16(vm);
                printf("%08d: %s\t0x%04X\t", IP(vm), opToStr(inst), oper);
                printValue(vm, getVal(vm->val_store, oper));
                break;
            }

            case OP_CONSTANT: {
                printf("%08d: %s\t\t", IP(vm), opToStr(inst));
                uint16_t type = READ16(vm);
                uint64_t data = READ64(vm);
                Value obj;
                assignVal(&obj, type, &data);
                printValue(vm, obj);
                break;
            }

            // 16 bit signed operand
            case OP_JMP:
            case OP_JMPIF: {
                int oper = (int)READ16(vm);
                printf("%08d: %s\t%d\t", IP(vm), opToStr(inst), oper);
                printValue(vm, getVal(vm->val_store, oper));
                break;
            }

            // 32 bit operand
            case OP_EXCEPT:
                printf("%08d: %s\t0x%08X", IP(vm), opToStr(inst), READ32(vm));
                break;

            default:
                printf("invalid instruction: 0x%02X at 0x%08X", inst, IP(vm));
                exit(1);

        }
    }
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "use: %s filename\n", argv[0]);
        exit(1);
    }

    VirtualMachine* vm = loadVM(argv[1]);
    disassemble(vm);

    return 0;
}