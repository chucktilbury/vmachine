
// stand-alone disassembler.
#include "common.h"
#include <stdarg.h>

#include "vm_support.h"

void printline(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
}

void disassemble(VirtualMachine* vm)
{
    bool finished = false;
    int inst = 0;

    while(!finished) {
        inst = read8(vm);
        //printf("instruction: %s\n", opToStr(instruction));
        switch(inst) {
            case OP_EXIT:
                printline("%08d:%12s",
                        getIndex(vm->inst), opToStr(inst));
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
            case OP_NOT_EQ:
            case OP_LESS_EQ:
            case OP_GTR_EQ:
            case OP_LESS:
            case OP_GREATER:
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
            case OP_PRINT: {
                printline("%08d:%12s",
                            getIndex(vm->inst), opToStr(inst));
                break;
            }

            // 16 bit operand
            case OP_CALL:
            case OP_CALLX:
            case OP_PUSH:
            case OP_ALLOC:
            case OP_FREE:
                printline("%08d:%12s\t0x%04X",
                            getIndex(vm->inst),
                            opToStr(inst), read16(vm));
                break;

            // 16 bit signed operand
            case OP_JMP:
            case OP_JMPIF:
                printline("%08d:%12s\t%d",
                            getIndex(vm->inst),
                            opToStr(inst), (int)read16(vm));
                break;

            // 32 bit operand
            case OP_EXCEPT:
                printline("%08d:%12s\t0x%08X",
                            getIndex(vm->inst),
                            opToStr(inst), read32(vm));
                break;

            default:
                printline("invalid instruction: 0x%02X at 0x%08X",
                            inst, getIndex(vm->inst));
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