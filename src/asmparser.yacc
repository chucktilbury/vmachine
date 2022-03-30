/*
 * This is the complete grammar for the NOP programming language.
 */

%debug
%defines
%locations

%{

#include <math.h>
#include <stdarg.h>

#include "common.h"
#include "vm_support.h"
#include "scanner.h"

// defined by flex
extern int yylex(void);
extern int yyparse(void);
extern FILE *yyin;
void yyerror(const char* s);

int error_count = 0;

void addSymbol(const char* name, int slot);
int findSymbol(const char* name);

VirtualMachine* vm = NULL;

#define TOKSTR get_tok_str()

void syntaxError(const char* fmt, ...)
{
    fprintf(stderr, "Syntax Error: %s: line %d: at %d: ", get_file_name(), get_line_number(), get_col_number());
    error_count++;

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
}

void addVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum + right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum + (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum + (uint64_t)((int64_t)right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum + (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum + right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum + (int64_t)right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum + (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum + (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum + right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }
}

void subVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum - right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum - (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum - (uint64_t)((int64_t)right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum - (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum - right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum - (int64_t)right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum - (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum - (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum - right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }
}

void mulVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum * right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum * (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum * (uint64_t)((int64_t)right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum * (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum * right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum * (int64_t)right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum * (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum * (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum * right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }
}

void divVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum / right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum / (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum / (uint64_t)((int64_t)right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum / (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum / right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum / (int64_t)right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = left.data.fnum / (double)right.data.unum; break;
                case VAL_INUM: dest->data.fnum = left.data.fnum / (double)right.data.inum; break;
                case VAL_FNUM: dest->data.fnum = left.data.fnum / right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }
}

void modVals(Value* dest, Value left, Value right)
{
    dest->type = left.type;
    switch(right.type) {
        case VAL_UNUM:
            if(right.data.unum == 0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        case VAL_INUM:
            if(right.data.inum == 0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        case VAL_FNUM:
            if(right.data.fnum == 0.0) {
                syntaxError("divide by zero");
                return;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }

    switch(left.type) {
        case VAL_UNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.unum = left.data.unum % right.data.unum; break;
                case VAL_INUM: dest->data.unum = left.data.unum % (uint64_t)right.data.inum; break;
                case VAL_FNUM: dest->data.unum = left.data.unum % (uint64_t)((int64_t)right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_INUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.inum = left.data.inum % (int64_t)right.data.unum; break;
                case VAL_INUM: dest->data.inum = left.data.inum % right.data.inum; break;
                case VAL_FNUM: dest->data.inum = left.data.inum % (int64_t)right.data.fnum; break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        case VAL_FNUM:
            switch(right.type) {
                case VAL_UNUM: dest->data.fnum = fmod(left.data.fnum, (double)right.data.unum); break;
                case VAL_INUM: dest->data.fnum = fmod(left.data.fnum, (double)right.data.inum); break;
                case VAL_FNUM: dest->data.fnum = fmod(left.data.fnum, right.data.fnum); break;
                default: syntaxError("only numbers allowed in expressions"); break;
            }
            break;
        default:
            syntaxError("only numbers allowed in expressions");
            break;
    }
}

void negVal(Value* dest, Value val)
{
    dest->type = val.type;
    switch(val.type) {
        case VAL_UNUM: dest->data.unum = (uint64_t)(-(int64_t)val.data.unum); break;
        case VAL_INUM: dest->data.unum = -val.data.inum; break;
        case VAL_FNUM: dest->data.unum = -val.data.fnum; break;
        default: syntaxError("only numbers allowed in expressions"); break;
    }
}

%}

%define parse.error verbose
%locations

%union {
    int type;
    uint64_t unum;
    int64_t inum;
    double fnum;
    char *str;
    int opcode;
    Value value;
};

%token <str> TOK_SYMBOL
%token <unum> TOK_UNUM
%token <inum> TOK_INUM
%token <fnum> TOK_FNUM
%token <str> TOK_STR
%token <opcode> TOK_ERROR TOK_NOOP TOK_EXIT TOK_CALL TOK_CALLR TOK_CALLX TOK_RETURN
%token <opcode> TOK_JMP TOK_JMPR TOK_JMPIF TOK_JMPIFR TOK_EXCEPT TOK_PUSH TOK_POP
%token <opcode> TOK_NOT TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_LESS TOK_PRINT
%token <opcode> TOK_GTR TOK_NEG TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_MOD
%token <type> TOK_UNUM_TYPE TOK_INUM_TYPE TOK_FNUM_TYPE
%token <type> TOK_STR_TYPE
%token TOK_INCLUDE

%type <type> type_spec
%type <value> expression expression_factor
%type <str> label

%right '='
%left '+' '-'
%left '*' '/' '%'
%left NEG

%%
    /*
        Module rules.
     */
program
    : module_item_list
    ;

module_item_list
    : module_item
    | module_item_list module_item
    ;

module_item
    : label
    | include_statement
    | data_definition
    | instruction_block
    ;

label
    : TOK_SYMBOL {
        Value val;
        initValue(&val, VAL_INDEX);
        val.data.unum = getIndex(vm->inst);
        val.name = $1;
        val.isAssigned = true;
        int slot = addValStore(vm->val_store, val);
        addSymbol($1, slot);
    }
    ;

type_spec
    : TOK_UNUM_TYPE
    | TOK_INUM_TYPE
    | TOK_FNUM_TYPE
    ;

include_statement
    : TOK_INCLUDE TOK_STR { open_file($2); }
    ;

data_definition
    : type_spec TOK_SYMBOL {
        Value val;
        initValue(&val, $1);
        val.name = $2;
        val.isAssigned = false;
        int slot = addValStore(vm->val_store, val);
        addSymbol($2, slot);
    }
    | type_spec TOK_SYMBOL '=' expression {
        Value val;
        initValue(&val, $1);
        val.name = $2;
        val.isAssigned = true;
        switch($1) {
            case VAL_UNUM: val.data.unum = $4.data.unum; break;
            case VAL_INUM: val.data.inum = $4.data.inum; break;
            case VAL_FNUM: val.data.fnum = $4.data.fnum; break;
        }
        int slot = addValStore(vm->val_store, val);
        addSymbol($2, slot);
        //printValue(val);
    }
    | TOK_STR_TYPE TOK_SYMBOL '=' TOK_STR {
        Value val;
        initValue(&val, $1);
        val.name = $2;
        val.isAssigned = true;
        val.data.str = $4;
        int slot = addValStore(vm->val_store, val);
        addSymbol($2, slot);
    }
    ;

    /* instructions that have no operand */
class1_instruction
    : TOK_EXIT { WRITE8(vm, OP_EXIT); }
    | TOK_NOOP { WRITE8(vm, OP_NOOP); }
    | TOK_RETURN { WRITE8(vm, OP_RETURN); }
    | TOK_POP { WRITE8(vm, OP_POP); }
    | TOK_NOT { WRITE8(vm, OP_NOT); }
    | TOK_NEG { WRITE8(vm, OP_NEG); }
    | TOK_EQ { WRITE8(vm, OP_EQ); }
    | TOK_NEQ { WRITE8(vm, OP_NEQ); }
    | TOK_LEQ { WRITE8(vm, OP_LEQ); }
    | TOK_GEQ { WRITE8(vm, OP_GEQ); }
    | TOK_LESS { WRITE8(vm, OP_LESS); }
    | TOK_GTR { WRITE8(vm, OP_GTR); }
    | TOK_ADD { WRITE8(vm, OP_ADD); }
    | TOK_SUB { WRITE8(vm, OP_SUB); }
    | TOK_MUL { WRITE8(vm, OP_MUL); }
    | TOK_DIV { WRITE8(vm, OP_DIV); }
    | TOK_MOD { WRITE8(vm, OP_MOD); }
    | TOK_PRINT { WRITE8(vm, OP_PRINT); }
    ;

    /* instructions that have an immediate operand */
class2_instruction
    : TOK_CALL TOK_SYMBOL {
        WRITE8(vm, OP_CALL);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_CALLR TOK_SYMBOL {
        WRITE8(vm, OP_CALLR);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_JMP TOK_SYMBOL {
        WRITE8(vm, OP_JMP);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_JMPR TOK_SYMBOL {
        WRITE8(vm, OP_JMPR);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_JMPIF TOK_SYMBOL {
        WRITE8(vm, OP_JMPIF);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_JMPIFR TOK_SYMBOL {
        WRITE8(vm, OP_JMPIFR);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_CALLX TOK_SYMBOL {
        WRITE8(vm, OP_CALLX);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_PUSH TOK_SYMBOL {
        WRITE8(vm, OP_PUSH);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_EXCEPT TOK_SYMBOL {
        WRITE8(vm, OP_EXCEPT);
        WRITE16(vm, findSymbol($2));
    }
    | TOK_ERROR TOK_SYMBOL {
        WRITE8(vm, OP_ERROR);
        WRITE16(vm, findSymbol($2));
    }
    ;

instruction_block
    : label instruction_list
    ;

instruction_item
    : class1_instruction
    | class2_instruction
    ;

instruction_list
    : instruction_item
    | instruction_list instruction_item
    ;

expression_factor
    : TOK_SYMBOL {
        int slot = findSymbol($1);
        Value val = getValStore(vm->val_store, slot);
        if(!val.isAssigned)
            syntaxError("symbol \"%s\" has not been assigned a value", $1);
        memcpy(&$$, &val, sizeof(Value));
    }
    | TOK_UNUM {
        $$.data.unum = $1;
        $$.type = VAL_UNUM;
    }
    | TOK_INUM {
        $$.data.inum = $1;
        $$.type = VAL_INUM;
    }
    | TOK_FNUM {
        $$.data.fnum = $1;
        $$.type = VAL_FNUM;
    }
    | TOK_STR {
        // this is here to produce an error
        $$.data.str = $1;
        $$.type = VAL_STRING;
    }
    ;

expression
    : expression_factor
    | expression '+' expression { addVals(&$$, $1, $3); }
    | expression '-' expression { subVals(&$$, $1, $3); }
    | expression '*' expression { mulVals(&$$, $1, $3); }
    | expression '/' expression { divVals(&$$, $1, $3); }
    | expression '%' expression { modVals(&$$, $1, $3); }
    | '-' expression %prec NEG { negVal(&$$, $2); }
    ;

%%

#include <stdio.h>

extern char yytext[];

// simple binary tree
typedef struct _sym_tab_ {
    const char* name;
    int slot;
    struct _sym_tab_* left;
    struct _sym_tab_* right;
} SymbolTable;

SymbolTable* symtab = NULL;

void yyerror(const char* s)
{
    fflush(stderr);
    syntaxError("%s", s);
}

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
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        fprintf(stderr, "USE: %s infile outfile\n\n", argv[0]);
        exit(1);
    }

    open_file(argv[1]);
    vm = createVirtualMachine();
    yyparse();
    if(error_count == 0)
        saveVM(argv[2], vm);

    return error_count;
}

