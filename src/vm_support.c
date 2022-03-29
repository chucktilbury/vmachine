
#include "common.h"
#include "vm_support.h"



VirtualMachine* createVirtualMachine()
{
    VirtualMachine* vm = malloc(sizeof(VirtualMachine));
    assert(vm != NULL);

    vm->inst = createInstStore();
    vm->val_stack = createObjStore();
    vm->val_store = createObjStore();
    vm->heap = createObjStore();

    return vm;
}

void destroyVirtualMachine(VirtualMachine* vm)
{
    if(vm != NULL) {
        destroyInstStore(vm->inst);
        destroyObjStore(vm->val_stack);
        destroyObjStore(vm->val_store);
        destroyObjStore(vm->heap);
        free(vm);
    }
}

void runMachine(VirtualMachine* vm)
{
    bool finished = false;
    int op = 0;

    while(!finished) {
        op = READ8(vm);
        //printf(" opcode: %s\n", opToStr(op));
        switch(op) {
            case OP_ERROR:
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                runtimeError("encountered error instruction");
                break;

            case OP_EXIT:
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                finished = true;
                break;

            // no operand
            case OP_NOOP:
            case OP_RETURN:
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                break;

            case OP_POP: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                Value obj;
                POP(vm, obj);
                (void)obj;
                break;
            }

            // no operand binary operation pops 2 values and pushes a
            // boolean result.
            case OP_EQ:
            case OP_NOT_EQ:
            case OP_LESS_EQ:
            case OP_GTR_EQ:
            case OP_LESS:
            case OP_GREATER: {
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                break;
            }

            // no operand binary operation pops 2 operands and pushes a
            // numeric value as the result.
            case OP_ADD: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                Value left, right, result;
                POP(vm, right);
                POP(vm, left);
                memcpy(&result, &right, sizeof(Value));
                result.data.inum = left.data.inum + right.data.inum;
                PUSH(vm, result);
                break;
            }

            case OP_DIV: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                Value left, right, result;
                POP(vm, right);
                POP(vm, left);
                memcpy(&result, &right, sizeof(Value));
                result.data.inum = left.data.inum / right.data.inum;
                PUSH(vm, result);
                break;
            }

            case OP_SUB:
            case OP_MUL:
            case OP_MOD: {
                // no operand
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                break;
            }

            // print the obj on top of the val_stack
            case OP_PRINT: {
                VTRACE(5, "%08d %s", IP(vm), opToStr(op));
                //marker("%08d %s\t", IP(vm), opToStr(op));
                Value result;
                PEEK(vm, result);
                printValue(result);
                break;
            }

            // 16 bit operand
            case OP_PUSH: {
                uint16_t idx = READ16(vm);
                VTRACE(5, "%08d %s\t%08d", IP(vm), opToStr(op), idx);
                COPY(vm, idx); // copy from val_store to val_stack
                break;
            }

            // create an object from a constant on the instruction stream
            // and push it on the value stack.
            case OP_ASSIGN: {
                uint16_t type = READ16(vm);
                uint64_t value = READ64(vm);
                Value obj;
                assignObj(&obj, type, &value);
                PUSH(vm, obj);
                break;
            }

            case OP_FREE: {
                VTRACE(5, "%08d %s\t%08d", IP(vm), opToStr(op), READ16(vm));
                break;
            }

            // 32 bit signed operand loads the instruction pointer
            case OP_CALL:
            case OP_CALLX:
            case OP_JMP:
            case OP_JMPIF: {
                VTRACE(5, "%08d %s\t%d", IP(vm), opToStr(op), (int)READ32(vm));
                break;
            }

            // 32 bit unsigned operand
            case OP_EXCEPT:
                VTRACE(5, "%08d %s\t0x%08X", IP(vm), opToStr(op), READ32(vm));
                break;

            default:
                runtimeError("invalid opcode: 0x%02X at 0x%08X", op, IP(vm));
        }
    }
}
