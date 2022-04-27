

#include "common.h"
#include "asm.h"

// defined in asm.c
extern Symbol* sym_table;

int add(Symbol* tree, Symbol* node)
{
    int val = strcmp(tree->key, node->key);
    if(val > 0) {
        if(tree->right != NULL) {
            return add(tree->right, node);
        }
        else {
            tree->right = node;
            return 1;
        }
    }
    else if(val < 0) {
        if(tree->left != NULL) {
            return add(tree->left, node);
        }
        else {
            tree->left = node;
            return 1;
        }
    }
    else {
        return 0;    // node already exists
    }
}

static uint16_t find(Symbol* tree, const char* key)
{
    int val = strcmp(tree->key, key);
    if(val > 0) {
        if(tree->right != NULL) {
            return find(tree->right, key);
        }
        else {
            return 0;    // not found
        }
    }
    else if(val < 0) {
        if(tree->left != NULL) {
            return find(tree->left, key);
        }
        else {
            return 0;    // not found
        }
    }
    else {
        return tree->idx;
    }
}

static void destroy(Symbol* tree)
{
    if(tree->right != NULL) {
        destroy(tree->right);
    }

    if(tree->left != NULL) {
        destroy(tree->left);
    }

    _free((void*)tree->key);
    _free(tree);
}

void addSymbol(const char* key, uint16_t idx)
{
    Symbol* obj = _alloc_ds(Symbol);
    obj->idx = idx;
    obj->key = strdup(key);
    obj->len = strlen(key);
    // info from the scanner
    obj->line = get_line_number();
    obj->col = get_col_number();
    obj->filename = get_file_name();

    if(sym_table != NULL) {
        add(sym_table, obj);
    }
    else {
        sym_table = obj;
    }
}

uint16_t findSymbol(const char* key)
{
    if(sym_table != NULL) {
        return find(sym_table, key);
    }
    else {
        return 0;    // error value
    }
}

void destroySymbols()
{
    if(sym_table != NULL) {
        destroy(sym_table);
    }
}

static void dump(Symbol* sym)
{
    if(sym->right != NULL) {
        dump(sym->right);
    }

    if(sym->left != NULL) {
        dump(sym->left);
    }

    printf("   key: %-15s index: %-4u ", sym->key, sym->idx);
    fflush(stdout); // not printing in the specified order
    printVar(0, getVar(sym->idx));
    printf("\n");
}

void dumpSymbols(int level)
{
    if(level <= getTraceLevel()) {
        printf("\nSymbol Table:\n");
        dump(sym_table);
    }
}


