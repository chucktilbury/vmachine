
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
        if(!strcmp(sym->key, "entry")) {
            fprintf(stderr, "Syntax Error: program \"entry\" point is not defined\n");
        }
        else {
            fprintf(stderr,
                "Syntax Error: %s: line %d: at %d: symbol \"%s\" is defined but never assigned a value\n",
                sym->filename, sym->line, sym->col, sym->key);
        }
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
    cmd_line cl = create_cmd_line("Virtual machine assembler.");
    add_str_param(cl, "outfile", "-o", "name of the output file", "out.run", 0);
    add_num_param(cl, "verbo", "-v", "set verbosity of listing file", 0, 0);
    add_none_param(cl, "listing", "-l", "create the listing", 0);
    add_str_param(cl, "lstfile", "-n", "name of the listing file", "stdout", 0);
    add_none_param(cl, "force", "-f", "force listing, even if there are errors", 0);
    add_none_param(cl, "debug", "-d", "output debugging information", 0);
    parse_cmd_line(cl, argc, argv);

    setTraceLevel(get_num_param(cl, "verbo"));

    reset_cmd_excess(cl);
    open_file(iterate_cmd_excess(cl));
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

