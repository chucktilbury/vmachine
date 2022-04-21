
#include <stdarg.h>

#include "common.h"
#include "vmachine.h"
#include "parser.h"
#include "asm.h"

extern int error_count;
Symbol* sym_table = NULL;

#include <stdio.h>

extern char yytext[];

static void verifySym(Symbol* sym)
{
    if(sym->right != NULL) {
        verifySym(sym->right);
    }

    if(sym->left != NULL) {
        verifySym(sym->left);
    }

    Variable* var = getVar(sym->idx);
    if(!var->isAssigned) {
        fprintf(stderr,
                "Syntax Error: %s: line %d: at %d: is defined but never assigned a value\n",
                sym->filename, sym->line, sym->col);
        error_count++;
    }
}

void verifySymbolTable()
{
    // TODO: add the "start" symbol and require it.
    verifySym(sym_table);
}

void yyerror(const char* s)
{
    fflush(stderr);
    syntaxError("%s", s);
}

int main(int argc, char** argv)
{
    // if(argc < 3) {
    //     fprintf(stderr, "USE: %s infile outfile\n\n", argv[0]);
    //     exit(1);
    // }
    cmd_line cl = create_cmd_line("Virtual machine assembler.");
    add_str_param(cl, "outfile", "-o", "name of the output file", "out.bin", 0);
    add_none_param(cl, "listing", "-l", "create the listing", 0);
    parse_cmd_line(cl, argc, argv);

    reset_cmd_excess(cl);
    open_file(iterate_cmd_excess(cl));
    //open_file(get_str_param(cl, "infile"));
    createVMachine();
    yyparse();
    if(error_count == 0) {
        saveVM(get_str_param(cl, "outfile"));
    }

    if(get_none_param(cl, "listing") && error_count == 0) {
        showListing();
    }

    destroyVMachine();
    destroySymbols(sym_table);
    destroy_cmd_line(cl);

    printf("error count = %d\n", error_count);
    return error_count;
}

