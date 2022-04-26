
#include "common.h"
#include "vmachine.h"
#include "symbols.h"

typedef struct _label_ {
    int index;
    const char* name;
    struct _label_* left;
    struct _label_* right;
} Name;

static Name* label;
static Name* syms;

// defined in asm.c
extern Symbol* sym_table;


/**
 * Traverse the symbol table and store the addresses in the label table.
 */
static void add(Name* label, Name* node)
{
    if(label->index > node->index) {
        if(label->right == NULL) {
            label->right = node;
        }
        else {
            add(label->right, node);
        }
    }
    else if(label->index < node->index) {
        if(label->left == NULL) {
            label->left = node;
        }
        else {
            add(label->left, node);
        }
    }
    else {
        return;
    }
}

static void add_sym(const char* name, uint16_t idx)
{
    Name* lab = _alloc_ds(Name);
    lab->name = _strdup(name);
    lab->index = idx;
    lab->left = NULL;
    lab->right = NULL;

    if(syms != NULL) {
        add(syms, lab);
    }
    else {
        syms = lab;
    }
}

static void add_label(uint16_t idx, const char* name)
{
    Name* lab = _alloc_ds(Name);
    lab->name = _strdup(name);
    lab->index = idx;
    lab->left = NULL;
    lab->right = NULL;

    if(label != NULL) {
        add(label, lab);
    }
    else {
        label = lab;
    }
}

static void do_symbols(Symbol* node)
{
    if(node->left != NULL) {
        do_symbols(node->left);
    }
    if(node->right != NULL) {
        do_symbols(node->right);
    }

    Variable* var = getVar(node->idx);
    add_sym(node->key, node->idx);

    if(var->type == VAL_ADDRESS) {
        //printf("adding: %s %d\n", node->key, node->idx);
        add_label(var->data.addr, node->key);
    }
}

static void destroy(Name* node)
{
    if(node->left != NULL) {
        destroy(node->left);
    }
    if(node->right != NULL) {
        destroy(node->right);
    }

    _free((void*)node->name);
    _free(node);
}

static const char* find(Name* node, int index)
{
    if(node->index == index) {
        return node->name;
    }
    else if(node->index > index) {
        if(node->right != NULL) {
            return find(node->right, index);
        }
        else {
            return NULL;
        }
    }
    else {
        if(node->left != NULL) {
            return find(node->left, index);
        }
        else {
            return NULL;
        }
    }
}

static const char* find_sym(int index)
{
    return find(syms, index);
}

static const char* find_label(int index)
{
    return find(label, index);
}

void showListing()
{
    int inst = 0;

    do_symbols(sym_table);

    while(getIndex() < getLen()) {
        int idx = getIndex();
        const char* lab = find_label(idx);
        if(lab != NULL) {
            printf("\n----: %s\n", lab);
        }

        inst = read8();
        printf("%04d: %s\t", idx, opToStr(inst));

        switch(inst) {
            case OP_EXIT:
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
                break;

            case OP_PRINTS:
                printf("----\t");
                break;

            case OP_PRINT: {
                    uint16_t oper = read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%4d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                }
                break;

            // 16 bit operand
            case OP_CALL: {
                    uint16_t oper = read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%4d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                }
                break;

//             case OP_CALL8: {
//                     uint8_t type = read8();
//                     uint8_t valu = read8();
//                     printf("%s\t%4d\t", varTypeToStr(type), valu);
//                 }
//                 break;
//
//             case OP_CALL16: {
//                     uint8_t type = read8();
//                     uint16_t valu = read16();
//                     printf("%s\t%4d\t", varTypeToStr(type), valu);
//                 }
//                 break;
//
//             case OP_CALL32: {
//                     uint8_t type = read8();
//                     uint32_t valu = read32();
//                     printf("%s\t%4d\t", varTypeToStr(type), valu);
//                 }
//                 break;

            case OP_CALLX:
            case OP_SAVE:
            case OP_PUSH: {
                    uint16_t oper = read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%4d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                }
                break;

            case OP_PEEK: {
                    short ofst = (short)read16();
                    printf("%d\t", ofst);
                }
                break;

            case OP_LOCAL: {
                    short ofst = (short)read16();
                    printf("%d\t", ofst);
                }
                break;

            case OP_PUSH8: {
                    uint8_t type = read8();
                    uint32_t valu = read8();
                    // printf("%s\t%4d\t", varTypeToStr(type), valu);
                    printf("----\t");
                    printVal(type, &valu);
                }
                break;

            case OP_PUSH16: {
                    uint8_t type = read8();
                    uint32_t valu = read16();
                    //printf("%s\t%4d\t", varTypeToStr(type), valu);
                    printf("----\t");
                    printVal(type, &valu);
                }
                break;

            case OP_PUSH32: {
                    uint8_t type = read8();
                    uint32_t valu = read32();
                    printf("----\t"); //%s\t%4d\t", varTypeToStr(type), valu);
                    printVal(type, &valu);
                }
                break;

            // 16 bit signed operand
            case OP_JMP: {
                    uint16_t oper = read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%4d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                }
                break;

//             case OP_JMP8: {
//                     uint8_t valu = read8();
//                     printf("%4d\t", valu);
//                 }
//                 break;
//
//             case OP_JMP16: {
//                     uint16_t valu = read16();
//                     printf("%4d\t", valu);
//                 }
//                 break;
//
//             case OP_JMP32: {
//                     uint32_t valu = read32();
//                     printf("%4d\t", valu);
//                 }
//                 break;

            case OP_JMPIF: {
                    int oper = (int)read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                }
                break;

//             case OP_JMPIF8: {
//                     uint8_t valu = read8();
//                     printf("%4d\t", valu);
//                 }
//                 break;
//
//             case OP_JMPIF16: {
//                     uint16_t valu = read16();
//                     printf("%4d\t", valu);
//                 }
//                 break;
//
//             case OP_JMPIF32: {
//                     uint32_t valu = read32();
//                     printf("%4d\t", valu);
//                 }
//                 break;

            case OP_EXCEPT:
                printf("0x%08X", read16());
                break;

            case OP_CAST: {
                    int type = (int)read8();
                    int oper = (int)read16();
                    const char* str = find_sym(oper);
                    if(str == NULL) {
                        printf("%4d\t", oper);
                        printVar(getVar(oper));
                    }
                    else {
                        printf("%s\t", str);
                    }
                    printf("to %s", varTypeToStr(type));
                }
                break;

            default:
                printf("invalid instruction: 0x%02X at 0x%0d\n", inst, getIndex());
                exit(1);

        }
        printf("\n");
    }

    destroy(label);
    destroy(syms);

    dumpSymbols();
    dumpVarStore();
    dumpStrStore();
}
