
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void runtimeError(const char* fmt, ...)
{
    fprintf(stderr, "Runtime Error: ");
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
    exit(1);
}

void genericError(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    runtimeError(fmt, args);
    va_end(args);
}