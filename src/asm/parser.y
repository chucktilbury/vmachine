/*
 * This is the complete grammar for the NOP programming language.
 */

%debug
%defines
%locations

%{

#include "common.h"
#include <stdarg.h>

#include "scanner.h"
#include "arith_expr.h"
#include "jumps.h"
//#include "expressions.h"

// defined by flex
extern int yylex(void);
extern int yyparse(void);
extern FILE *yyin;
void yyerror(const char* s);

extern VMachine* vm;
extern Symbol* sym_table;
extern int error_count;
void syntaxError(const char*, ...);

#define TOKSTR get_tok_str()

%}

%define parse.error verbose
%locations

%union {
    int type;
    uint32_t unum;
    int32_t inum;
    float fnum;
    int opcode;
    char* str;
    Value* value;
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
%token <type> TOK_STR_TYPE TOK_BOOL_TYPE TOK_CAST TOK_TRUE TOK_FALSE
%token TOK_INCLUDE

%type <value> type_spec expression expression_factor
%type <str> label
%type <type> bool_val

%right '='
%left '+' '-'
%left '*' '/' '%'
%left NEGATE

%%
program
    : module_item_list
    ;

module_item_list
    : module_item
    | module_item_list module_item
    ;

module_item
    : label
    | instruction_block
    | include_statement
    | data_definition
    ;

label
    : TOK_SYMBOL {
        // create an address object
        if(findSymbol($1))
            syntaxError("symbol \"%s\" has already been defined", $1);
        else {
            Value* val = createVal(VAL_ADDRESS);
            val->isAssigned = true;
            val->isConst = true;
            val->isLiteral = true;
            val->data.unum = vm->inst->len;
            addSymbol($1, addVal(vm->val_store, val));
        }
    }
    ;

type_spec
    : TOK_UNUM_TYPE {
        $$ = createVal(VAL_UNUM);
    }
    | TOK_INUM_TYPE {
        $$ = createVal(VAL_INUM);
    }
    | TOK_FNUM_TYPE {
        $$ = createVal(VAL_FNUM);
    }
    | TOK_CONST TOK_UNUM_TYPE {
        Value* val = createVal(VAL_UNUM);
        val->isConst = true;
        $$ = val;
    }
    | TOK_CONST TOK_INUM_TYPE {
        Value* val = createVal(VAL_INUM);
        val->isConst = true;
        $$ = val;
    }
    | TOK_CONST TOK_FNUM_TYPE {
        Value* val = createVal(VAL_FNUM);
        val->isConst = true;
        $$ = val;
    }
    ;

include_statement
    : TOK_INCLUDE TOK_STR { open_file($2); }
    ;

bool_val
    : TOK_TRUE { $$ = $1; }
    | TOK_FALSE { $$ = $1; }
    ;

data_definition
    : type_spec TOK_SYMBOL {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else
            addSymbol($2, addVal(vm->val_store, $1));
    }
    | type_spec TOK_SYMBOL '=' expression {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            $1->isAssigned = true;
            assignVal($1, $4);
            addSymbol($2, addVal(vm->val_store, $1));
        }
    }
    | type_spec TOK_SYMBOL '=' TOK_SYMBOL {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            $1->isAssigned = true;
            assignVal($1, getVal(vm->val_store, findSymbol($4)));
            addSymbol($2, addVal(vm->val_store, $1));
        }
    }
    | TOK_BOOL_TYPE TOK_SYMBOL '=' bool_val {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            Value* val = createVal(VAL_BOOL);
            val->data.boolean = ($4 == TOK_TRUE)? true: false;
            val->isAssigned = true;
            addSymbol($2, addVal(vm->val_store, val));
        }
    }
    | TOK_STR_TYPE TOK_SYMBOL '=' TOK_STR {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            Value* val = createVal(VAL_OBJ);
            //Index idx = addStr(vm->str_store, $4);
            //initVal(&val, $1);
            // TODO: FIX ME.
            val->isAssigned = true;
            val->data.obj = 0; //idx;
            addSymbol($2, addVal(vm->val_store, val));
        }
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
        emitCALL(vm, $2);
    }
    | TOK_CALL TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_CALL);
            WRITE16(vm, slot);
        }
    }
    | TOK_JMP expression {
        emitJMP(vm, $2);
    }
    | TOK_JMP TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_JMP);
            WRITE16(vm, slot);
        }
    }
    | TOK_JMPIF expression {
        emitJMPIF(vm, $2);
    }
    | TOK_JMPIF TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_JMPIF);
            WRITE16(vm, slot);
        }
    }
    | TOK_PUSH expression {
        emitPUSH(vm, $2);
    }
    | TOK_PUSH TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_PUSH);
            WRITE16(vm, slot);
        }
    }
    ;

class4_instruction
    : TOK_CALLX TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_CALLX);
            WRITE16(vm, slot);
        }
    }
    | TOK_EXCEPT TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_EXCEPT);
            WRITE16(vm, slot);
        }
    }
    | TOK_ERROR TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_ERROR);
            WRITE16(vm, slot);
        }
    }
    | TOK_SAVE TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_SAVE);
            WRITE16(vm, slot);
        }
    }
    ;


class3_instruction
    : TOK_CAST TOK_SYMBOL type_spec {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            WRITE8(vm, OP_CAST);
            WRITE8(vm, $3->type);
            WRITE16(vm, slot);
        }
    }
    ;

instruction_block
    : label instruction_list
    ;

instruction_item
    : class1_instruction
    | class2_instruction
    | class3_instruction
    | class4_instruction
    ;

instruction_list
    : instruction_item
    | instruction_list instruction_item
    ;

    /* removed symbol and string, but these will have to be addressed later */
    /* If a value is to be derived from a symbol, then that has to be done using
        assembly language. */
expression_factor
    : TOK_UNUM {
        $$ = createVal(VAL_UNUM);
        $$->data.unum = $1;
        $$->isLiteral = true;
    }
    | TOK_INUM {
        $$ = createVal(VAL_INUM);
        $$->data.inum = $1;
        $$->isLiteral = true;
    }
    | TOK_FNUM {
        $$ = createVal(VAL_FNUM);
        $$->data.fnum = $1;
        $$->isLiteral = true;
    }
    ;

expression
    : expression_factor
    | expression '+' expression { addVals($$, $1, $3); }
    | expression '-' expression { subVals($$, $1, $3); }
    | expression '*' expression { mulVals($$, $1, $3); }
    | expression '/' expression { divVals($$, $1, $3); }
    | expression '%' expression { modVals($$, $1, $3); }
    | '-' expression %prec NEGATE { negVal($2); $$ = $2; }
    | '(' expression ')' { $$ = $2; }
    ;

%%
