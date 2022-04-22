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
#include "emit.h"
#include "symbols.h"

// defined by flex
extern int yylex(void);
extern int yyparse(void);
extern FILE *yyin;
void yyerror(const char* s);

// defined in main.c
extern Symbol* sym_table;
extern int error_count;
void syntaxError(const char*, ...);
void verifySymbolTable();

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
    Variable* value;
    uint16_t string;
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
%token <opcode> TOK_PRINTS TOK_PEEK TOK_LOCAL
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
    : module_item_list {
        verifySymbolTable();
    }
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
        int slot = findSymbol($1);
        if(slot != 0) {
            Variable* val = getVar(slot);
            if(val->isAssigned)
                syntaxError("symbol \"%s\" has already been defined", $1);
            else {
                val->isAssigned = true;
                val->isConst = true;
                val->isLiteral = true;
                val->data.unum = getLen();
            }
        }
        else {
            Variable* val = createVar(VAL_ADDRESS);
            val->isAssigned = true;
            val->isConst = true;
            val->isLiteral = true;
            val->data.unum = getLen();
            addSymbol($1, addVar(val));
        }
    }
    ;

type_spec
    : TOK_UNUM_TYPE {
        $$ = createVar(VAL_UNUM);
    }
    | TOK_INUM_TYPE {
        $$ = createVar(VAL_INUM);
    }
    | TOK_FNUM_TYPE {
        $$ = createVar(VAL_FNUM);
    }
    | TOK_CONST TOK_UNUM_TYPE {
        Variable* var = createVar(VAL_UNUM);
        var->isConst = true;
        $$ = var;
    }
    | TOK_CONST TOK_INUM_TYPE {
        Variable* var = createVar(VAL_INUM);
        var->isConst = true;
        $$ = var;
    }
    | TOK_CONST TOK_FNUM_TYPE {
        Variable* var = createVar(VAL_FNUM);
        var->isConst = true;
        $$ = var;
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
            addSymbol($2, addVar($1));
    }
    | type_spec TOK_SYMBOL '=' expression {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            assignVar($1, $4);
            addSymbol($2, addVar($1));
        }
    }
    | type_spec TOK_SYMBOL '=' TOK_SYMBOL {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            assignVar($1, getVar(findSymbol($4)));
            addSymbol($2, addVar($1));
        }
    }
    | TOK_BOOL_TYPE TOK_SYMBOL '=' bool_val {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            Variable* val = createVar(VAL_BOOL);
            val->data.boolean = ($4 == TOK_TRUE)? true: false;
            val->isAssigned = true;
            addSymbol($2, addVar(val));
        }
    }
    | TOK_STR_TYPE TOK_SYMBOL '=' TOK_STR {
        if(findSymbol($2))
            syntaxError("symbol \"%s\" has already been defined", $2);
        else {
            Variable* val = createVar(VAL_OBJ);
            //uint16_t idx = addStr(vm->str_store, $4);
            //initVal(&val, $1);
            // TODO: FIX ME.
            val->isAssigned = true;
            val->data.obj = 0; //idx;
            addSymbol($2, addVar(val));
        }
    }
    ;

    /* instructions that have no operand */
class1_instruction
    : TOK_EXIT { write8(OP_EXIT); }
    | TOK_NOOP { write8(OP_NOOP); }
    | TOK_RETURN { write8(OP_RETURN); }
    | TOK_POP { write8(OP_POP); }
    | TOK_NOT { write8(OP_NOT); }
    | TOK_NEG { write8(OP_NEG); }
    | TOK_EQ { write8(OP_EQ); }
    | TOK_NEQ { write8(OP_NEQ); }
    | TOK_LEQ { write8(OP_LEQ); }
    | TOK_GEQ { write8(OP_GEQ); }
    | TOK_LESS { write8(OP_LESS); }
    | TOK_GTR { write8(OP_GTR); }
    | TOK_ADD { write8(OP_ADD); }
    | TOK_SUB { write8(OP_SUB); }
    | TOK_MUL { write8(OP_MUL); }
    | TOK_DIV { write8(OP_DIV); }
    | TOK_MOD { write8(OP_MOD); }
    | TOK_PRINTS { write8(OP_PRINTS); }
    ;

    /* instructions that have an immediate operand. the operand is a
        constant value */
class2_instruction
    : TOK_CALL TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0) {
            Variable* val = createVar(VAL_ADDRESS);
            slot = addVar(val);
            addSymbol($2, slot);
        }
        write8(OP_CALL);
        write16(slot);
    }
    | TOK_JMP TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0) {
            Variable* val = createVar(VAL_ADDRESS);
            slot = addVar(val);
            addSymbol($2, slot);
        }
        write8(OP_JMP);
        write16(slot);
    }
    | TOK_JMPIF TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0) {
            Variable* val = createVar(VAL_ADDRESS);
            slot = addVar(val);
            addSymbol($2, slot);
        }
        write8(OP_JMPIF);
        write16(slot);
    }
    | TOK_PUSH expression {
        emitPUSH($2);
    }
    | TOK_PUSH TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0) {
            syntaxError("undefined symbol: \"%s\"", $2);
        }
        else {
            write8(OP_PUSH);
            write16(slot);
        }
    }
    ;

class3_instruction
    : TOK_CALLX TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            write8(OP_CALLX);
            write16(slot);
        }
    }
    | TOK_EXCEPT TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            write8(OP_EXCEPT);
            write16(slot);
        }
    }
    | TOK_ERROR TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            write8(OP_ERROR);
            write16(slot);
        }
    }
    | TOK_SAVE TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            write8(OP_SAVE);
            write16(slot);
            Variable* var = getVar(slot);
            var->isAssigned = true;
        }
    }
    | TOK_PRINT TOK_SYMBOL {
        int slot = findSymbol($2);
        if(slot == 0) {
            syntaxError("undefined symbol: \"%s\"", $2);
        }
        else {
            write8(OP_PRINT);
            write16(slot);
        }
    }
    ;


class4_instruction
    : TOK_CAST TOK_SYMBOL type_spec {
        int slot = findSymbol($2);
        if(slot == 0)
            syntaxError("undefined symbol: \"%s\"", $2);
        else {
            write8(OP_CAST);
            write8($3->type);
            write16(slot);
        }
    }
    | TOK_PEEK expression {
        emitPeek($2);
    }
    | TOK_LOCAL expression {
        emitLocal($2);
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
        $$ = createVar(VAL_UNUM);
        $$->data.unum = $1;
        $$->isLiteral = true;
    }
    | TOK_INUM {
        $$ = createVar(VAL_INUM);
        $$->data.inum = $1;
        $$->isLiteral = true;
    }
    | TOK_FNUM {
        $$ = createVar(VAL_FNUM);
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
