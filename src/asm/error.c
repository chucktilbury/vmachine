
#include "common.h"
#include <stdarg.h>

#include "scanner.h"
#include "asm.h"

int error_count = 0;

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

void genericError(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    syntaxError(fmt, args);
    va_end(args);
}