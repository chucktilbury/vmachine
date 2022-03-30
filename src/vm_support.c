
#include "common.h"
#include "vm_support.h"



VirtualMachine* createVirtualMachine()
{
    VirtualMachine* vm = _alloc_ds(VirtualMachine);

    vm->inst = createInstStore();
    vm->val_stack = createValStore();
    vm->val_store = createValStore();
    vm->heap = createValStore();
    vm->strs = createStrStore();

    return vm;
}

void destroyVirtualMachine(VirtualMachine* vm)
{
    if(vm != NULL) {
        destroyInstStore(vm->inst);
        destroyValStore(vm->val_stack);
        destroyValStore(vm->val_store);
        destroyValStore(vm->heap);
        destroyStrStore(vm->strs);
        _free(vm);
    }
}

#define ABINARY(op) do { \
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode)); \
                Value left, right, result; \
                POP(vm, right); \
                POP(vm, left); \
                memcpy(&result, &right, sizeof(Value)); \
                result.data.inum = left.data.inum op right.data.inum; \
                PUSH(vm, result); } while(0)

#define CBINARY(op) do {\
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode)); \
                } while(0)

void runMachine(VirtualMachine* vm)
{
    bool finished = false;
    int opcode = 0;

    while(!finished) {
        opcode = READ8(vm);
        //printf(" opcode: %s\n", opToStr(opcode));
        switch(opcode) {
            case OP_ERROR:
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                runtimeError("encountered error instruction");
                break;

            case OP_EXIT:
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                finished = true;
                break;

            // no operand
            case OP_NOOP:
            case OP_RETURN:
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                break;

            case OP_POP: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                Value obj;
                POP(vm, obj);
                (void)obj;
                break;
            }

            // no operand binary operation pops 2 values and pushes a
            // boolean result.
            case OP_EQ:         CBINARY(==); break;
            case OP_NEQ:     CBINARY(!=); break;
            case OP_LEQ:    CBINARY(<=); break;
            case OP_GEQ:     CBINARY(>=); break;
            case OP_LESS:       CBINARY(<); break;
            case OP_GTR:    CBINARY(>); break;

            // no operand binary operation pops 2 operands and pushes a
            // numeric value as the result.
            case OP_ADD:    ABINARY(+); break;
            case OP_SUB:    ABINARY(-); break;
            case OP_MUL:    ABINARY(*); break;
            case OP_DIV:    ABINARY(/); break;
            case OP_MOD:    ABINARY(%); break;

            // print the obj on top of the val_stack
            case OP_PRINT: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                //marker("%08d %s\t", IP(vm), opToStr(opcode));
                Value result;
                PEEK(vm, result);
                printValue(result);
                break;
            }

            // 16 bit operand
            case OP_PUSH: {
                uint16_t idx = READ16(vm);
                VTRACE(5, "%08d %s\t%08d", IP(vm), opToStr(opcode), idx);
                COPY(vm, idx); // copy from val_store to val_stack
                break;
            }

            // create an object from a constant on the instruction stream
            // and push it on the value stack.
            case OP_CONSTANT: {
                uint16_t type = READ16(vm);
                uint64_t value = READ64(vm);
                VTRACE(5, "%08d %s\t%s", IP(vm), opToStr(opcode), valToStr(type));
                Value obj;
                assignValue(&obj, type, &value);
                printValue(obj);
                PUSH(vm, obj);
                break;
            }

            case OP_FREE: {
                VTRACE(5, "%08d %s\t%08d", IP(vm), opToStr(opcode), READ16(vm));
                break;
            }

            // 32 bit signed operand loads the instruction pointer
            case OP_CALL:
            case OP_CALLX:
            case OP_JMP:
            case OP_JMPIF: {
                VTRACE(5, "%08d %s\t%d", IP(vm), opToStr(opcode), (int)READ32(vm));
                break;
            }

            // 32 bit unsigned operand
            case OP_EXCEPT:
                VTRACE(5, "%08d %s\t0x%08X", IP(vm), opToStr(opcode), READ32(vm));
                break;

            default:
                runtimeError("invalid opcode: 0x%02X at 0x%08X", opcode, IP(vm));
        }
    }
}
