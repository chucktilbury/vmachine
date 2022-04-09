
#include "common.h"
#include <stdarg.h>
#include "vmachine.h"
#include "scanner.h"
#include "parser.h"

int error_count = 0;
VMachine* vm = NULL;
Symbol* sym_table = NULL;

#include <stdio.h>

extern char yytext[];

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
    add_str_param(cl, "infile" , "-i", "name of the input file", NULL, CF_REQD);
    add_str_param(cl, "outfile", "-o", "name of the output file", "t.out", 0);
    parse_cmd_line(cl, argc, argv);

    open_file(get_str_param(cl, "infile"));
    vm = createVMachine();
    yyparse();
    if(error_count == 0) {
        saveVM(vm, get_str_param(cl, "outfile"));
    }

    dumpSymbols(vm);

    destroyVMachine(vm);
    destroySymbols(sym_table);

    printf("error count = %d\n", error_count);
    return error_count;
}

