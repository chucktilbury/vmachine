
#include "common.h"
#include "vm_support.h"
#include "pars_support.h"



VMachine* createVMachine()
{
    VMachine* vm = _alloc_ds(VMachine);

    vm->inst = createInstStore();
    vm->val_stack = createValStore();
    vm->val_store = createValStore();
    vm->str_store = createStrStore();

    return vm;
}

void destroyVMachine(VMachine* vm)
{
    if(vm != NULL) {
        destroyInstStore(vm->inst);
        destroyValStore(vm->val_stack);
        destroyValStore(vm->val_store);
        destroyStrStore(vm->str_store);
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

void runMachine(VMachine* vm)
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
                }
                break;

            // no operand binary operation pops 2 values and pushes a
            // boolean result.
            case OP_EQ:     CBINARY(==); break;
            case OP_NEQ:    CBINARY(!=); break;
            case OP_LEQ:    CBINARY(<=); break;
            case OP_GEQ:    CBINARY(>=); break;
            case OP_LESS:   CBINARY(<);  break;
            case OP_GTR:    CBINARY(>);  break;

            // no operand binary operation pops 2 operands and pushes a
            // numeric value as the result.
            case OP_ADD: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value left, right, result;
                    POP(vm, left);
                    POP(vm, right);
                    addVals(&result, right, left);
                    PUSH(vm, result);
                }
                break;

            case OP_SUB: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value left, right, result;
                    POP(vm, left);
                    POP(vm, right);
                    subVals(&result, right, left);
                    PUSH(vm, result);
                }
                break;

            case OP_MUL: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value left, right, result;
                    POP(vm, left);
                    POP(vm, right);
                    mulVals(&result, right, left);
                    PUSH(vm, result);
                }
                break;

            case OP_DIV: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value left, right, result;
                    POP(vm, left);
                    POP(vm, right);
                    divVals(&result, right, left);
                    PUSH(vm, result);
                }
                break;

            case OP_MOD: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value left, right, result;
                    POP(vm, left);
                    POP(vm, right);
                    modVals(&result, right, left);
                    PUSH(vm, result);
                }
                break;


            // print the obj on top of the val_stack
            case OP_PRINT: {
                    VTRACE(5, "%08d %s", IP(vm), opToStr(opcode));
                    Value result;
                    PEEK(vm, result);
                    printVal(vm, result);
                }
                break;

            // 16 bit operand
            case OP_PUSH: {
                    uint16_t idx = READ16(vm);
                    VTRACE(5, "%08d %s\t%08d", IP(vm), opToStr(opcode), idx);
                    COPY(vm, idx); // copy from val_store to val_stack
                }
                break;

            // copy a var from the top of stack into the value pool.
            case OP_SAVE: {
                    Index idx = READ16(vm);
                    Value val;
                    PEEK(vm, val);
                    assignVal(&vm->val_store->list[idx], &val);
                }
                break;

            // 32 bit signed operand loads the instruction pointer
            case OP_CALL:
            case OP_CALLX:
            case OP_JMP:
            case OP_JMPIF: {
                    VTRACE(5, "%08d %s\t%d", IP(vm), opToStr(opcode), (int)READ32(vm));
                }
                break;

            // 32 bit unsigned operand
            case OP_EXCEPT:
                VTRACE(5, "%08d %s\t0x%08X", IP(vm), opToStr(opcode), READ32(vm));
                break;

            case OP_SETZ:
                vm->zflag = true;
                break;

            case OP_CLRZ:
                vm->zflag = false;
                break;

            default:
                runtimeError("invalid opcode: 0x%02X at 0x%08X", opcode, IP(vm));
        }
    }
}
