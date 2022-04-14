
#include "common.h"
#include "vmachine.h"
#include "symbols.h"

void showListing()
{
    bool finished = false;
    int inst = 0;

    while(!finished) {
        printf("%04d: ", getIndex());
        inst = read8();
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
            case OP_CALL: {
                    uint16_t oper = read16();
                    printf("%s\t%4d\t", opToStr(inst), oper);
                    printVar(getVar(oper));
                }
                break;

            case OP_CALL8: {
                    uint8_t type = read8();
                    uint8_t valu = read8();
                    printf("%s\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                }
                break;

            case OP_CALL16: {
                    uint8_t type = read8();
                    uint16_t valu = read16();
                    printf("%s\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                }
                break;

            case OP_CALL32: {
                    uint8_t type = read8();
                    uint32_t valu = read32();
                    printf("%s\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                }
                break;

            case OP_CALLX:
            case OP_SAVE:
            case OP_PUSH: {
                    uint16_t oper = read16();
                    printf("%s\t%4d\t", opToStr(inst), oper);
                    printVar(getVar(oper));
                }
                break;

            case OP_PUSH8: {
                    uint8_t type = read8();
                    uint8_t valu = read8();
                    printf("%s\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                }
                break;

            case OP_PUSH16: {
                    uint8_t type = read8();
                    uint16_t valu = read16();
                    printf("%s\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                }
                break;

            case OP_PUSH32: {
                    uint8_t type = read8();
                    uint32_t valu = read32();
                    printf("%s\t\t%s\t%4d\n", opToStr(inst), varTypeToStr(type), valu);
                    // TODO: display the proper type
                }
                break;

            // 16 bit signed operand
            case OP_JMP: {
                    uint16_t oper = read16();
                    printf("%s\t%4d\t", opToStr(inst), oper);
                    printVar(getVar(oper));
                }
                break;

            case OP_JMP8: {
                    uint8_t valu = read8();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_JMP16: {
                    uint16_t valu = read16();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_JMP32: {
                    uint32_t valu = read32();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_JMPIF: {
                    int oper = (int)read16();
                    printf("%s\t%d\t", opToStr(inst), oper);
                    printVar(getVar(oper));
                }
                break;

            case OP_JMPIF8: {
                    uint8_t valu = read8();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_JMPIF16: {
                    uint16_t valu = read16();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_JMPIF32: {
                    uint32_t valu = read32();
                    printf("%s\t%4d\n", opToStr(inst), valu);
                }
                break;

            case OP_EXCEPT:
                printf("%s\t0x%08X", opToStr(inst), read16());
                break;

            case OP_CAST: {
                    int type = (int)read8();
                    int oper = (int)read16();
                    printf("%s\t%d\t", opToStr(inst), oper);
                    printVar(getVar(oper));
                    printf("          cast to %s\n", varTypeToStr(type));
                }
                break;

            default:
                printf("invalid instruction: 0x%02X at 0x%0d\n", inst, getIndex());
                exit(1);

        }
    }

    dumpSymbols();
}
