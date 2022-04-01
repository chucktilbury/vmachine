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
    int opcode;
    char* str;
    Value value;
    Index string;
};

%token <str> TOK_SYMBOL
%token <unum> TOK_UNUM
%token <inum> TOK_INUM
%token <fnum> TOK_FNUM
%token <str> TOK_STR
%token <opcode> TOK_ERROR TOK_NOOP TOK_EXIT TOK_CALL TOK_CALLX TOK_RETURN
%token <opcode> TOK_JMP TOK_JMPIF TOK_EXCEPT TOK_PUSH TOK_POP TOK_SAVE
%token <opcode> TOK_NOT TOK_EQ TOK_NEQ TOK_LEQ TOK_GEQ TOK_LESS TOK_PRINT
%token <opcode> TOK_GTR TOK_NEG TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_MOD
%token <type> TOK_UNUM_TYPE TOK_INUM_TYPE TOK_FNUM_TYPE TOK_CONST
%token <type> TOK_STR_TYPE TOK_BOOL_TYPE
%token TOK_INCLUDE

%type <value> type_spec expression expression_factor
%type <string> label

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
        // create an address object
        Value val;
        initVal(&val, VAL_ADDRESS);
        val.isAssigned = true;
        val.isConst = true;
        val.data.unum = vm->inst->len;
        addSymbol(vm->sym_table, $1, addVal(vm->const_store, val));
    }
    ;

type_spec
    : TOK_UNUM_TYPE {
        Value val;
        initVal(&val, VAL_UNUM);
        $$ = val;
    }
    | TOK_INUM_TYPE {
        Value val;
        initVal(&val, VAL_INUM);
        $$ = val;
    }
    | TOK_FNUM_TYPE {
        Value val;
        initVal(&val, VAL_FNUM);
        $$ = val;
    }
    | TOK_BOOL_TYPE {
        Value val;
        initVal(&val, VAL_BOOL);
        $$ = val;
    }
    | TOK_CONST TOK_UNUM_TYPE {
        Value val;
        initVal(&val, VAL_UNUM);
        val.isConst = true;
        $$ = val;
    }
    | TOK_CONST TOK_INUM_TYPE {
        Value val;
        initVal(&val, VAL_INUM);
        val.isConst = true;
        $$ = val;
    }
    | TOK_CONST TOK_FNUM_TYPE {
        Value val;
        initVal(&val, VAL_FNUM);
        val.isConst = true;
        $$ = val;
    }
    | TOK_CONST TOK_BOOL_TYPE {
        Value val;
        initVal(&val, VAL_BOOL);
        val.isConst = true;
        $$ = val;
    }
    ;

include_statement
    : TOK_INCLUDE TOK_STR { open_file($2); }
    ;

data_definition
    : type_spec TOK_SYMBOL {
        // create an uninitialized object of the given type and store the
        // name into the symbol table.
        addSymbol(vm->sym_table, $2, addVal(vm->val_store, $1));
    }
    | type_spec TOK_SYMBOL '=' expression {
        // create a numeric value of the given type and store it in the value
        // store, then store the symbol in the symbol table.
        $1.isAssigned = true;
        assignVal(&$1, &$4);
        addSymbol(vm->sym_table, $2, addVal(vm->val_store, $1));
    }
    | TOK_STR_TYPE TOK_SYMBOL '=' TOK_STR {
        // Add the string to the string store, then create the value object
        // that references it. Then store the name in the symbol table with
        // the slot number of the value (not the string)
        Value val;
        Index idx = addStr(vm->str_store, $4);
        initVal(&val, $1);
        val.isAssigned = true;
        val.data.obj = idx;
        addSymbol(vm->sym_table, $2, addVal(vm->val_store, val));
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

    /* instructions that have an immediate operand. the operand is a
        constant value */
class2_instruction
    : TOK_CALL expression {
        WRITE8(vm, OP_CALL);
        WRITE16(vm, addVal(vm->const_store, $2));
    }
    | TOK_JMP expression {
        WRITE8(vm, OP_JMP);
        WRITE16(vm, addVal(vm->const_store, $2));
    }
    | TOK_JMPIF expression {
        WRITE8(vm, OP_JMPIF);
        WRITE16(vm, addVal(vm->const_store, $2));
    }
    | TOK_CALLX expression {
        WRITE8(vm, OP_CALLX);
        WRITE16(vm, addVal(vm->const_store, $2));
    }
    | TOK_EXCEPT expression {
        WRITE8(vm, OP_EXCEPT);
        WRITE16(vm, addVal(vm->const_store, $2));
    }
    | TOK_ERROR expression {
        WRITE8(vm, OP_ERROR);
        WRITE16(vm, addVal(vm->const_store, $2));
    }

    /* Instructions that have have an operand that is a variable value. */
class3_instruction
    : TOK_PUSH TOK_SYMBOL {
        WRITE8(vm, OP_PUSH);
        WRITE16(vm, findSymbol(vm->sym_table, $2));
    }
    | TOK_SAVE TOK_SYMBOL {
        WRITE8(vm, OP_SAVE);
        WRITE16(vm, findSymbol(vm->sym_table, $2));
    }
    ;

instruction_block
    : label instruction_list
    ;

instruction_item
    : class1_instruction
    | class2_instruction
    | class3_instruction
    ;

instruction_list
    : instruction_item
    | instruction_list instruction_item
    ;

expression_factor
    : TOK_SYMBOL {
        // find the value associated with the symbol and use it in the
        // expression.
        int slot = findSymbol(vm->sym_table, $1);
        Value val = getVal(vm->val_store, slot);
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
        $$.data.obj = addStr(vm->str_store, $1);
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

void yyerror(const char* s)
{
    fflush(stderr);
    syntaxError("%s", s);
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
        saveVM(vm, argv[2]);

    return error_count;
}

