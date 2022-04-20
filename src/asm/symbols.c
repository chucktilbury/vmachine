

#include "common.h"
#include "scanner.h"
#include "symbols.h"

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
    printVar(getVar(sym->idx));
    printf("\n");
}

void dumpSymbols()
{
    printf("\nSymbol Table:\n");
    dump(sym_table);
}


/* // simple binary tree
typedef struct _sym_tab_ {
    const char* name;
    int slot;
    struct _sym_tab_* left;
    struct _sym_tab_* right;
} SymbolTable;

SymbolTable* symtab = NULL;

void add_symbol(SymbolTable* root, SymbolTable* sym)
{
    int val = strcmp(root->name, sym->name);
    if(val < 0) {
        if(root->left == NULL)
            root->left = sym;
        else
            add_symbol(root->left, sym);
    }
    else if(val > 0) {
        if(root->right == NULL)
            root->right = sym;
        else
            add_symbol(root->right, sym);
    }
    else
        syntaxError("symbol \"%s\" already exists.", sym->name);
}

void addSymbol(const char* name, int slot)
{
    SymbolTable* symbol = malloc(sizeof(SymbolTable));
    assert(symbol != NULL);

    symbol->name = name;
    symbol->slot = slot;
    symbol->left = NULL;
    symbol->right = NULL;
    if(symtab != NULL)
        add_symbol(symtab, symbol);
    else
        symtab = symbol;
}

int find_symbol(SymbolTable* node, const char* name)
{
    int val = strcmp(node->name, name);
    if(val < 0) {
        if(node->left != NULL)
            return find_symbol(node->left, name);
        else {
            syntaxError("symbol \"%s\" is undefined.", name);
            return -1;
        }
    }
    else if(val > 0) {
        if(node->right != NULL)
            return find_symbol(node->right, name);
        else {
            syntaxError("symbol \"%s\" is undefined.", name);
            return -1;
        }
    }
    else
        return node->slot;
}

int findSymbol(const char* name)
{
    return find_symbol(symtab, name);
} */

