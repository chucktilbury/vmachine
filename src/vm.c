
#include "common.h"
#include "vm_support.h"

const int TRACE_level = 5;

#include <stdarg.h>

void dump_buffer(uint8_t* buffer, int size) {

    for(int i = 0; i < size; i++)
        printf(", 0x%02X", buffer[i]);
    printf("\n");
}

void runMachine(VirtualMachine* vm)
{
    bool finished = false;
    int op = 0;

    while(!finished) {
        op = read8InstStore(vm->inst);
        //printf(" opcode: %s\n", opToStr(op));
        switch(op) {
            case OP_ERROR:
                // no operand
                runtimeError("encountered error instruction");
                break;
            case OP_NOOP:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_EXIT:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                finished = true;
                break;
            case OP_CALL:
                // 16 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%04X", read16InstStore(vm->inst));
                break;
            case OP_CALLX:
                // 16 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%04X", read16InstStore(vm->inst));
                break;
            case OP_RETURN:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_JMP:
                // 16 bit signed operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: %d", (int)read16InstStore(vm->inst));
                break;
            case OP_JMPIF:
                // 16 bit signed operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: %d", (int)read16InstStore(vm->inst));
                break;
            case OP_EXCEPT:
                // 32 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%08X", read32InstStore(vm->inst));
                break;
            case OP_PUSH:
                // 16 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%04X", read16InstStore(vm->inst));
                break;
            case OP_POP:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_ALLOC:
                // 16 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%04X", read16InstStore(vm->inst));
                break;
            case OP_FREE:
                // 16 bit operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: 0x%04X", read16InstStore(vm->inst));
                break;
            case OP_EQ:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_NOT_EQ:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_LESS_EQ:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_GTR_EQ:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_LESS:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_GREATER:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_ADD:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_SUB:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_MUL:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_DIV:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_MOD:
                // no operand
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            case OP_PRINT: // print the obj on top of the var stack
                TRACE(5, " opcode: %s", opToStr(op));
                TRACE(5, "operand: none");
                break;
            default:
                runtimeError("invalid opcode: 0x%02X at 0x%08X", op, (uint64_t)vm->inst->index);
        }
    }
}

int virtualMachine(const char* fname)
{
    // load the file
    VirtualMachine* vm = loadVM(fname); //createVirtualMachine();

    // run the program
    runMachine(vm);
    return 0;
}

int main(int argc, char** argv)
{
    if(argc < 2) {
        fprintf(stderr, "use: %s filename\n", argv[0]);
        exit(1);
    }

    return virtualMachine(argv[1]);
}